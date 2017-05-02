/**
@file ChangeEntityTypeState.h

Contiene la declaración del componente que 
ontrola el movimiento de salir en el pozo

@see Logic::CChangeEntityTypeState
@see Logic::IComponent

@author Guidi Giacomo
@date June, 2015
*/
#ifndef __Logic_ChangeEntityTypeState_H
#define __Logic_ChangeEntityTypeState_H

#include "Logic/Entity/Component.h"


//declaración de la clase
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
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CChangeEntityTypeState();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);



		/**
		Este método se invoca en cada ciclo de la simulación y actualiza la posición del actor
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
