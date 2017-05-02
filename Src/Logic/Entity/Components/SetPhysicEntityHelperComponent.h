/**
@file SetPhysicEntityHelperComponent.h

Contains the declaration of the SetPhysicEntityHelper Component

@see Logic::CGraphics
@see Logic::IComponent

@author Rayco S�nchez Garc�a
@date March, 2015
*/
#ifndef __Logic_CSetPhysicEntityHelperComponent_H
#define __Logic_CSetPhysicEntityHelperComponent_H

#include "Logic/Entity/Component.h"

namespace Graphics
{
	class CMeshEntity;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	This component receives physic messages and changes the mesh material
	from graphic component for advicing the player where it is possible
	to set the entity.
	
    @ingroup logicGroup

	@author Rayco S�nchez Garc�a
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CSetPhysicEntityHelperComponent();
		
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

		virtual void awake();

		virtual bool activate();

		virtual void deactivate();

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

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
