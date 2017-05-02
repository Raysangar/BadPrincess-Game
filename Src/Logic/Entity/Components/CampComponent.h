/**
@file CampComponent.h

Contiene la declaraci�n del componente que 
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
//declaraci�n de la clase
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CCampComponent();
		
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
