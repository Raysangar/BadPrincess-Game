/**
@file RemovableWorldComponent.h


@see Logic::CGraphics
@see Logic::IComponent

@author Iván León
@date June, 2015
*/
#ifndef __Logic_RemovableWorldComponent_H
#define __Logic_RemovableWorldComponent_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que recibe la textura y e frustum para dibujar l'area de efecto en suelo
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class CRemovableWorldComponent : public IComponent
	{
		DEC_FACTORY(CRemovableWorldComponent);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CRemovableWorldComponent(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CRemovableWorldComponent();
		
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

		virtual void tick(unsigned int msecs);

		bool getActivateTransparancy() const { return _activateTransparency; }

		std::vector<std::string> getMaterials() const { return _materials; }

	protected:

		bool _activateTransparency;
		std::vector<std::string> _materials;

		std::vector<Ogre::Vector3*> _centers;		
		std::vector<float> _radiuses;		

	}; // class CGraphics

	REG_FACTORY(CRemovableWorldComponent);

} // namespace Logic

#endif // __Logic_Graphics_H
