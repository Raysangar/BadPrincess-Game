/**
@file SetPhysicEntityHelperComponent.h

Contains the declaration of the SetPhysicEntityHelper Component

@see Logic::CGraphics
@see Logic::IComponent

@author Rayco Sánchez García
@date March, 2015
*/
#ifndef __Logic_CSetPhysicEntityHelperComponent_H
#define __Logic_CSetPhysicEntityHelperComponent_H

#include "Logic/Entity/Component.h"

namespace Graphics
{
	class CMeshEntity;
}

//declaración de la clase
namespace Logic 
{
/**
	This component receives physic messages and changes the mesh material
	from graphic component for advicing the player where it is possible
	to set the entity.
	
    @ingroup logicGroup

	@author Rayco Sánchez García
	@date March, 2015
*/
	class CSetPhysicEntityHelperComponent : public IComponent
	{
		DEC_FACTORY(CSetPhysicEntityHelperComponent);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CSetPhysicEntityHelperComponent(std::string componentName) : IComponent(componentName),
			wrongPositionMaterial(""), correctPositionMaterial(""), isOnCorrectPosition(true),correctPositionLuaFunction("")

		{
		}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CSetPhysicEntityHelperComponent();
		
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

		virtual void awake();

		virtual bool activate();

		virtual void deactivate();

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);
		
		virtual void tick(unsigned int msecs);

		bool isPhysicEntityOnCorrectPosition();

		virtual bool checkIsCorrectPosition();
	private:


		Ogre::Quaternion currentPitch;

		void adaptEntityPitch();

		std::string wrongPositionMaterial;
		std::string correctPositionMaterial;
		std::string correctPositionLuaFunction;

		Graphics::CMeshEntity* meshEntity;

		bool isOnCorrectPosition;
	}; // class CGraphics

	REG_FACTORY(CSetPhysicEntityHelperComponent);

} // namespace Logic

#endif // __Logic_CSetPhysicEntityHelperComponent_H
