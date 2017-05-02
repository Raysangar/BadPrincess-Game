/**
@file UnitSelected.h

Contiene la declaración del componente que controla la representación
el estado de selección de una entidad controlable.

@see Logic::CGraphics
@see Logic::IComponent

@author Iván León
@date Enero, 2015
*/
#ifndef __Logic_UnitSelected_H
#define __Logic_UnitSelected_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CFrustumEntity;
	class CScene;
}

namespace Ogre
{
	class Vector2;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de gestionar si una entidad controlable está seleccionada 
	para recibir acciones o no. En caso positivo, dibuja un círculo bajo la misma.
	
    @ingroup logicGroup

	@author Iván León
	@date Enero, 2015
*/
	class CUnitSelected : public IComponent
	{
		DEC_FACTORY(CUnitSelected);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CUnitSelected(std::string componentName) : IComponent(componentName) , _selected(false), _active(false), _nodeActive(nullptr), _nodeSelected(nullptr),_currentNode(0),_size(Vector2::ZERO),_projectionHeight(0), 
			_scene(nullptr),_selectedMaterial(""),_activeMaterial("")
		{
		}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CUnitSelected();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el tamaño de la 
		cápsula que representa la entidad física y usa su radio para asignárselo
		al círculo de selección.
		Después crea un ManualObject para dibujar el círculo y lo asigna a un
		SceneNode de Ogre también recién creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
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
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posición.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

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

		virtual bool activate();

		virtual void deactivate();

		bool isSelected() { return _selected; }

	protected:
		
		void updateSize(Ogre::Vector2 size);
	
		void initDecal();
		/**
		Flag que indica si la unidad está seleccionada o no
		*/
		bool _selected;
		bool _active;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la creación y destrucción del círculo.
		*/
		Graphics::CScene* _scene;

		Vector2 _size;        // size of decal

		float _projectionHeight;
				
		std::string _activeMaterial;
		std::string _selectedMaterial;
 
		Graphics::CFrustumEntity *_currentNode;
		Graphics::CFrustumEntity *_nodeSelected;
		Graphics::CFrustumEntity *_nodeActive;
 
	    void addMaterial(std::string matName);

		void hide();
	}; // class CGraphics

	REG_FACTORY(CUnitSelected);

} // namespace Logic

#endif // __Logic_Graphics_H
