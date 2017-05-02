/**
@file PhysicEntity.h

Contiene la declaración del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicDebug
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_PhysicDebug_H
#define __Logic_PhysicDebug_H

#include "Physics.h"

// Predeclaración de tipos
namespace physx {
	class PxRigidActor;
	class PxRigidStatic;
	class PxShape;
};

namespace Physics {
	class CServer;
};

namespace Graphics 
{
	class CScene;
	class CManualEntity;
}
// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date Febrero, 2015
	*/
	class CPhysicDebug : public IComponent
	{
		DEC_FACTORY(CPhysicDebug);
	public:
		/**
		These constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CPhysicDebug(std::string componentName);

		/**
		Destructor. Delete the SceneNode that contains the physic shape the it has to draw. 
		*/
		virtual ~CPhysicDebug();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		this component accept only the ToggleDrawPhysicDebug.This message is used to toggle the visibility of physic shape.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		When recive the ToggleDrawPhysicDebug change the visibility of the physic shape
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		This method is invoked every simulation cicle and update the position and orientation of the shape if it is visible		
		*/
		virtual void tick(unsigned int msecs);
		/**
			This method is invoked when the entity is activate.
			In this moment all the components are already created and the thereby alse the possible physic actor
			This method get the physic actor from the CPhysicController or CPhysicEntity and try to draw the geometry with Ogre  
		*/
		virtual bool activate();

		virtual void deactivate();
	private:

		bool isVisible;

		Graphics::CScene* scene;
		//The Graphic Entity node that contain the shapes to draw
		Graphics::CManualEntity *debugEntity;
		
		/*
			Function that draw a Box.
			It create an Ogre item used to draw the shape

			@param actor The physic actor
			@param shape The shape to draw
		*/
		void drawBoxShape(const physx::PxRigidActor*actor,physx::PxShape* pShape);
		/*
			Function that draw a Capsule.
			It create an Ogre item used to draw the shape

			@param actor The physic actor
			@param shape The shape to draw
		*/
		void drawCapsuleShape(const physx::PxRigidActor*actor,physx::PxShape* pShape);
		
		/*
			Function that get the physic actor and cycle throw all its shapes and try to draw it

			@param actor The physic actor
		*/
		void drawActor(const physx::PxRigidActor*actor);
		/*
			Function that get the type of shape and try to draw it.
			Actually it can draw only Capsule and Box

			@param actor The physic actor
			@param shape The shape to draw
		*/
		void drawShape(const physx::PxRigidActor*actor,physx::PxShape* shape);

	}; // class CPhysicDebug

	REG_FACTORY(CPhysicDebug);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
