/**
@file CLACarryEntity.h

Represents the state of chasing an enemy

@author Guidi Giacomo
@date Mayo 2015


*/ 

#pragma once

#ifndef __AI_LACarryEntity_H
#define __AI_LACarryEntity_H

#include "LatentAction.h"

namespace Logic{

	class IComponent;
	class CCarrierState;
};

namespace AI 
{

	/**
	Esta acci�n persigue a una entidad rival hasta que se encuentra en rango de ataque
	*/
	class CLACarryEntity : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param entity Entidad due�a del estado
		*/
		CLACarryEntity(Logic::CEntity *entity) : CLatentAction(),_carrierState(nullptr)
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLACarryEntity() {};

		/**
		Al comenzar, se guardan referencias a los componentes que se utilizar�n
		m�s tarde.
		Tambi�n se guardan las variables necesarias para el OnRun();

		@return Estado de la funci�n; si se indica que la
		acci�n a terminado (LatentAction::Completed), se invocar�
		al OnStop().
		*/
		virtual LAStatus OnStart();

		/**
		M�todo invocado al final de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al final (y no durante toda la vida de la acci�n).

		En la mayor�a de los casos este m�todo no hace nada.
		*/
		virtual void OnStop();

		/**
		M�todo invocado c�clicamente para que se contin�e con la
		ejecuci�n de la acci�n.
		<p>
		En cada paso se comprueba si el enemigo asignado se encuentra en
		rango de ataque. Si es as�, finalizamos este estado y nos aseguramos
		de que la entidad se encuentra mirando hacia el enemigo.
		En caso contrario, se asigna un destino en rango de ataque a la entidad.

		@return Estado de la acci�n tras la ejecuci�n del m�todo;
		permite indicar si la acci�n ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun(unsigned int msecs) ;

		/**
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier raz�n).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;

		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		<p>		
		Aceptamos AvatarFinishedRoute y EnemyDismissed
		

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.
		Si recibimos un AvatarFinishedRoute quiere decir que hemos llegado
		a un punto dentro del rango de ataque. Pasamos al siguiente estado.
		Si recibimos EnemyDismissed quiere decir que, o bien a muerto, o ha salido del 
		trigger de visi�n. Pasamos a estado Idle
		

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	private:

		void init();

		void setupCarriedTransform();

		Logic::CCarrierState* _carrierState;

	
	};

} //namespace AI 

#endif // __AI_LACarryEntity_H