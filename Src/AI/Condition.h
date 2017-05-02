/**
@file CCondition.h

En este fichero se define la interfaz para las condiciones
de las aristas de las m�quinas de estado, as� como cuatro 
condiciones de ejemplo: CConditionTrue (una condici�n que 
siempre se eval�a como verdadera), CConditionFinished (es 
verdadera si la acci�n asociada al nodo ha terminado), 
CConditionSuccess (es  verdadera si la acci�n asociada al 
nodo ha terminado con �xito) y CConditionFail (es verdadera 
si la acci�n asociada al nodo ha terminado con fallo).

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_Condition_H
#define __AI_Condition_H

#include "BaseSubsystems\Math.h"
#include "LatentAction.h"

namespace Logic
{
	class Logic::CEntity;
};

namespace AI 
{
	/**
	Interfaz que deber�n implementar las condiciones de 
	las m�quinas de estado.
	*/
	template <class TNode>
	class ICondition
	{
	public:

		virtual ~ICondition(){}
		/**
		En este m�todo se comprobar� la condici�n. Recibe el 
		nodo al que pertenece la condici�n (de esta manera 
		podemos comprobar si el nodo ha terminado y en qu� 
		condiciones) y la entidad que ejecuta el comportamiento 
		(que puede ser NULL).
		
		@param currentNode Nodo al que pertenece la condici�n.
		@param entity Entidad que ejecuta el comportamiento
		@return true o false seg�n si se cumple la condici�n o no.
		*/
		virtual bool check(TNode* currentNode, Logic::CEntity* entity) = 0;
		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message) {return false;};
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message) {};
	}; // class CCondition

	/**
	Esta clase define una condici�n que siempre se eval�a a true.
	*/
	template <class TNode>
	class CConditionTrue : public ICondition<TNode>
	{
	public:
		bool check(TNode* currentNode, Logic::CEntity* entity) { return true; }
	};
	
	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado.
	*/
	class CConditionFinished : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{
			// Comprobar si el estado del nodo que se est� ejecutando es terminado
			return currentNode->getStatus() == CLatentAction::LAStatus::SUCCESS || 
				currentNode->getStatus() == CLatentAction::LAStatus::FAIL;

		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con �xito.
	*/
	class CConditionSuccess : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			// Comprobar si el estado del nodo que se est� ejecutando es �xito
			return currentNode->getStatus() == CLatentAction::LAStatus::SUCCESS;
		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con fallo.
	*/
	class CConditionFail : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			// Comprobar si el estado del nodo que se est� ejecutando es fallo
			return currentNode->getStatus() == CLatentAction::LAStatus::FAIL;
		}
	};

	/**
	Esta clase define una condici�n que devuelve true 
	si se recibi� un mensaje de un tipo concreto en el �ltimo tick
	*/
	template <class TNode>
	class CConditionMessage : public ICondition<TNode>
	{
	public:
		/**
		En el constructor recibimos el tipo de mensaje que 
		har� saltar la condici�n
		@param messageType Tipo de mensaje que estamos escuchando
		*/
		CConditionMessage(std::string messageType) : _received(false) {
			_messageType = messageType;
		}			

		/**
		En el check s�lo tenemos que comprobar el flag _received. Este flag
		se habr� activado en process si durante este tick hemos recibido
		un mensaje del tipo adecuado.
		Una vez hecha la comprobaci�n, reseteamos el flag a false.
		@param currentNode Nodo al que pertenece la condici�n (no se usa)
		@param entity Entidad que ejecuta el comportamiento (no se usa)
		@return true o false seg�n si se cumple la condici�n o no.

		*/
		bool check(TNode* currentNode, Logic::CEntity* entity) { 
			// Tenemos que comprobar el flag _received y luego actualizarlo
			// de nuevo a false para el siguiente tick
			if (_received)
			{
				_received = false;
				return true;
			}
			return false;
		}
		/**
		Devuelve true si el tipo del mensaje recibido coincide con el 
		que estamos esperando
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message) {
			// La condici�n debe aceptar mensajes del tipo indicado en _messageType
			return (message->type.compare(_messageType) == 0);
		};
		/**
		Procesa el mensaje recibido. Como s�lo aceptamos mensajes del
		tipo en el que estamos interesados aqu� directamente ponemos 
		el flag a true.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message) {
			// Actualizamos el flag _received si el mensaje es del tipo _messageType			
			_received = true;
		};

	private:
		/** Flag que se activa cuando recibimos un mensaje del tipo en el que estamos interesados */
		bool _received;
		/** Tipo del mensaje que esperamos */
		std::string _messageType;
	};


	/**
	Esta clase define una condici�n que se eval�a a true si la 
	entidad se encuentra en la posici�n indicada, con una tolerancia
	*/
	template <class TNode>
	class CConditionNear : public ICondition<TNode>
	{
	public:
		CConditionNear(Vector3 position, float tolerance) : _position(position), _tolerance(tolerance) {}

		bool check(CLatentAction* currentNode, Logic::CEntity* entity) 
		{ 
			// Tiene que devolver true cuando la posici�n de la entidad sea
			// igual a la posici�n que ha recibido como par�metro (con la tolerancia)				
			return entity->getPosition().positionCloses(_position);
		}

	protected:
		Vector3 _position;
		float _tolerance;
	};

} // namespace AI 

#endif // __AI_Condition_H
