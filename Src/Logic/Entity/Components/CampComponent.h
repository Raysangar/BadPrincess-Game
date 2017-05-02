/**
@file CampComponent.h

Contiene la declaración del componente que 
controla el spawn de enemigos si una unidad aliada se acerca a una posicion

@see Logic::CCampComponent
@see Logic::IComponent

@author Guidi Giacomo
@date June, 2015
*/
#ifndef __Logic_CampComponent_H
#define __Logic_CampComponent_H

#include "Logic/Entity/Component.h"

namespace AI
{
	class IActionNode;
}
//declaración de la clase
namespace Logic 
{
	class CEntity;

	class CCampComponent : public IComponent
	{

	DEC_FACTORY(CCampComponent);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CCampComponent(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CCampComponent();
		
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


	private:
		/*
		radio desde el centro de la entitad de spawn
		deberia ser un poco mas grande de la mitad del box de spawn
		o del radio del circulo de spawn
		*/
		float _campRange;
		
		bool _initiallyCampActived;

		int _initiallyWave;

		int _initiallyWaveMaxTime;

		bool _isCampActived;
		//wave to launch
		int _currentWave;

		int _currentWaveMaxTime;
		/*
			Function that get the visibility range and check if that range are close to the 
			current entity
		*/
		bool checkEntityDistance(Logic::CEntity* e);

		std::list<AI::IActionNode*> _waveActionList;
	}; 

	REG_FACTORY(CCampComponent);

} // namespace Logic

#endif // __Logic_CampComponent_H
