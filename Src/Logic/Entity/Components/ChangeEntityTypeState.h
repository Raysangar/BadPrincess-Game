/**
@file ChangeEntityTypeState.h

Contiene la declaraci�n del componente que 
ontrola el movimiento de salir en el pozo

@see Logic::CChangeEntityTypeState
@see Logic::IComponent

@author Guidi Giacomo
@date June, 2015
*/
#ifndef __Logic_ChangeEntityTypeState_H
#define __Logic_ChangeEntityTypeState_H

#include "Logic/Entity/Component.h"


//declaraci�n de la clase
namespace Logic 
{
	class CEntity;
	class CWorkmanState;

	class CChangeEntityTypeState : public IComponent
	{

	DEC_FACTORY(CChangeEntityTypeState);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CChangeEntityTypeState(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CChangeEntityTypeState();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		/**
		M�todo virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);



		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y actualiza la posici�n del actor
		*/
		virtual void tick(unsigned int msecs);

		Logic::CEntity* getWellEntity(){return _well;}

		void setWellEntity(Logic::CEntity* e){_well=e;}


		
	private:
		
	

		// Entidad pozo que tiene que cambiar la entidad.
		CEntity* _well;

		/*
		Function that set the well for changing the units type and destroy the goblin with particle
		*/
		void changeEntityType();

		bool _changeTypeActivated;
		int _timeToDestroy;

		std::string _changeTypeEntityParticleName;
		std::string _materialName;
	}; 

	REG_FACTORY(CChangeEntityTypeState);

} // namespace Logic

#endif // __Logic_ChangeEntityTypeState_H
