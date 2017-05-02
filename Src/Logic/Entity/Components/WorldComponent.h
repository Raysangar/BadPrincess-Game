/**
@file WorldComponent.h

Contiene la declaraci�n del componente que controla la representaci�n
el estado de selecci�n de una entidad controlable.

@see Logic::CGraphics
@see Logic::IComponent

@author Iv�n Le�n
@date Enero, 2015
*/
#ifndef __Logic_WorldComponent_H
#define __Logic_WorldComponent_H

#include "Logic/Entity/Component.h"

namespace Ogre{
	class Vector3;
};

namespace Graphics{
	class CFrustumEntity ;
	class CScene;
};

//declaraci�n de la clase
namespace Logic 
{
	class CEntity;
/**
	Componente que recibe la textura y e frustum para dibujar l'area de efecto en suelo
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class CWorldComponent : public IComponent
	{
		DEC_FACTORY(CWorldComponent);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CWorldComponent(std::string componentName) : IComponent(componentName), _elapsedTime(0), _destinationMarkNode(0),  _scene(0),_firstTick(true)
		{
		}

		/**
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CWorldComponent();
		
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
		
		virtual void awake();

		virtual void tick(unsigned int msecs);

		const Ogre::Vector3 getPhysicBounds() { return _physicBounds; } 

	protected:
		
		void initDecal(Graphics::CFrustumEntity* node, Vector2 size, float projectionHeight);
		void hide(Graphics::CFrustumEntity* node);


		Ogre::Vector3 _physicBounds;

		std::string _materialName;		

		bool _firstTick;

		int _elapsedTime;

		//Destination mark variables
		Graphics::CFrustumEntity *_destinationMarkNode;		
		Vector2 _destinationMarksize;        
		float _destinationMarkProjectionHeight;
		float _projectionHeight;
		std::string _destinationMarkMaterial;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la creaci�n y destrucci�n del c�rculo.
		*/
		Graphics::CScene* _scene;
		

	}; // class CGraphics

	REG_FACTORY(CWorldComponent);

} // namespace Logic

#endif // __Logic_Graphics_H
