/**
@file Entity.h

Contiene la declaración de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_Entity_H
#define __Logic_Entity_H

#include "BaseSubsystems/Math.h"

#include "Logic/Maps/EntityID.h"
#include "Logic/NMessage.h"

#include <list>
#include <string>
#include <memory> 

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}

namespace Logic 
{
	class CMap;
	class IComponent;
	class CEntityFactory;
}

namespace Ogre
{
	class Vector3;
}

class NMessage;

// Declaración de la clase
namespace Logic 
{
	/**
	Clase que representa una entidad en el entorno virtual. Las entidades
	son meros contenedores de componentes, y su funcionamiento depende
	de cuáles tenga. Además, la clase contiene una serie de atributos que
	pueden ser accedidos desde los componentes (Id, nombre, etc.).
	<p>
	Tras la creación de la entidad, donde se inicializan los valores de la
	entidad a su valor por defecto, se invocará a su método spawn() en el
	que se inicializarán los atributos de la entidad con los valores leidos 
	del fichero del mapa. Tras esto, en algún momento, cuando se active
	el mapa se llamará al método activate() de la entidad. En este momento
	los componentes harán visibles los componentes gráficos y demás ya que 
	apartir de ahí es cuando la entidad empieza su función siendo actualizada 
	por el tick() en cada vuelta de bucle.

    @ingroup logicGroup
    @ingroup entityGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CEntity 
	{
	protected:

		/**
		Clase amiga que crea y destruye objetos de la clase.
		*/
		friend class CEntityFactory;

		/**
		Constructor protegido de la clase (para crear CEntity debe
		utilizarse la factoría CEntityFactory). El constructor
		no hace más que poner el identificador único de la entidad
		, pues la inicialización efectiva se hace en el método spawn().
		
		@param entityID Identificador único de la entidad.
		*/
		CEntity(TEntityID entityID);

		/**
		Destructor de la clase. Es un método protegido pues para
		eliminar CEntity debe utilizarse la factoría
		CEntityFactory. El destructor invoca al destructor de
		cada componente.
		Cuando el destructor es invocado, éste _ya ha sido desenganchado
		del mapa al que perteneció_, por lo que los destructores de los
		componentes no pueden utilizar el mapa.
		*/
		~CEntity();

		/**
		Inicialización del objeto Logic, utilizando la información extraída de
		la entidad leída del mapa (Map::CEntity). Avisará a los componentes
		de la entidad para que se inicialicen.

		@param map Mapa Logic en el que se registrará la entidad.
		@param entity Información de construcción de la entidad leída del
		fichero de disco.
		@param newName Nombre para la entidad en caso de ser creada dinámicamente

		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		bool spawn(CMap *map, const Map::CEntity *entity, const std::string &newName = "", const Ogre::Vector3 &newPosition = Ogre::Vector3(0.0f,0.0f,0.0f));

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Calls awake method of all its components.
		*/
		void awake();
	public:

		/**
		Activa la entidad. Esto significa que el mapa ha sido activado.
		<p>
		El método llama al activate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true si todo fue bien.
		*/
		bool activate();


		/**
		Activates components which were activated before pausing the entity.

		@return Returns true if the activation of all component is succesful.
		*/
		bool restart();

		/**
		Deactivates active components remembering the state so when restart is called 
		only active components are reactivated.
		*/
		void pause();

		/**
		Desactiva la entidad. Esto significa que el mapa ha sido desactivado.
		<p>
		El método llama al deactivate() de todos los componentes para que
		se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Llamará a los métodos tick() de todos sus componentes.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Método que añade un nuevo componente a la lista de la entidad.

		@param component Componente a añadir.
		*/
		void addComponent(IComponent* component);

		/**
		Método que devuelve un componente de la lista de la entidad.

		@param type Tipo del componente a devolver.
		*/
		IComponent* CEntity::getComponent(std::string type);

		/**
		Method for deactivating one single component.

		@param componentType The component type that is wanted to deactivate.
		*/
		void deactivate(std::string componentType);

		/**
		Method for activating one single component.

		@param componentType The component type that is wanted to activate.
		*/
		void activate(std::string componentType);

		/**
		Método que quita un componente de la lista.

		El componente es eliminado de la lista de componentes de la
		entidad, pero <b>no</b> es desactivado ni eliminado (con delete);
		la responsabilidad de esa tarea se deja al invocante.

		@param component Componente a borrar.
		@return true si se borró el componente (false si el componente
		no estaba en el objeto).
		*/
		bool removeComponent(IComponent* component);
		
		/**
		Método que destruye todos los componentes de una entidad.
		*/
		void destroyAllComponents();

		/**
		Recibe un mensaje que envia a todos los componentes de la lista menos 
		al componente que lo envia, si éste se especifica en el segundo campo.

		@param message Mensaje a enviar.
		@param emitter Componente emisor, si lo hay. No se le enviará el mensaje.
		@return true si al menos un componente aceptó el mensaje
		*/
		bool emitMessageN(const std::shared_ptr<NMessage> &message, IComponent* emitter = 0);
		/**
		Devuelve el identificador único de la entidad.

		@return Identificador.
		*/
		Logic::TEntityID getEntityID() const { return _entityID; }

		/**
		Método que indica si la entidad es o no el jugador.
		Seguro que hay formas mejores desde el punto de vista de
		diseño de hacerlo, pero esta es la más rápida: la entidad 
		con la descripción de la entidad tiene esta descripción que
		establece en el spawn().
		
		@return true si la entidad es el jugador.
		*/
		bool isPlayer() { return _isPlayer; }

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		CMap *getMap() { return _map; }

		/**
		Devuelve el mapa donde está la entidad.

		@return Puntero al mapa que contiene la entidad.
		*/
		const CMap *getMap() const { return _map; }

		/**
		Devuelve el nombre de la entidad.

		@return Nombre de la entidad.
		*/
		const std::string &getName() const { return _name; }

		/**
		Devuelve el tipo de la entidad. Este atributo es leido de
		la entidad del mapa en spawn().

		@return Tipo de la entidad.
		*/
		const std::string &getType() const { return _type; }

		/**
		Establece la matriz de transformación de la entidad. Avisa a los 
		componentes del cambio.

		@param transform Nueva matriz de transformación de la entidad.
		*/
		void setTransformN(const Matrix4& transform);
		/**
		Devuelve la metriz de transformación de la entidad.
		<p>
		La posición es inicialmente leída del mapa (si no aparece,
		se colocará a (0, 0, 0)) y la orientación es también inicialmente 
		leída del mapa, como un simple viraje (si no aparece, se colocará 
		a 0). Obviamente, pueden cambiar con el tiempo.

		@return Matriz de transformación de la entidad en el entorno.
		*/
		Matrix4 getTransform() const { return _transform; }

		/**
		Establece la posición de la entidad. Avisa a los componentes
		del cambio.

		@param position Nueva posición.
		*/
		void setPositionN(const Vector3 &position, IComponent* invoker = 0);
		/**
		Devuelve la posición de la entidad.
		<p>
		La posición es inicialmente leída del mapa (si no aparece,
		se colocará a (0, 0, 0)), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Posición de la entidad en el entorno.
		*/
		Vector3 getPosition() const { return _transform.getTrans(); }

		/**
		Establece la orientación de la entidad. Avisa a los componentes
		del cambio.

		@param pos Nueva orientación.
		*/
		void setOrientationN(const Matrix3& orientation);
		/**
		Devuelve la matriz de rotación de la entidad.
		<p>
		La orientación es inicialmente leída del mapa como un simple 
		viraje (si no aparece, se colocará a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Orientación en el entorno.
		*/
		Matrix3 getOrientation() const;

		/**
		Establece el viraje de la entidad. Avisa a los componentes
		del cambio.

		@param yaw Nuevo viraje.
		*/
		void setYawN(float yaw);
		/**
		Vira la entidad. Avisa a los componentes del cambio.

		@param yaw Viraje a aplicar.
		*/
		void yawN(float yaw);

		/**
		Devuelve el viraje de la entidad.
		<p>
		La orientación es inicialmente leída del mapa como un simple 
		viraje (si no aparece, se colocará a 0), aunque, obviamente, puede
		cambiar con el tiempo.

		@return Viraje en el entorno.
		*/
		float getYaw() const { return Math::getYaw(_transform); }

		/**
		Indica si la entidad se encuentra activa.

		@return true si la entidad está activa.
		*/
		bool isActivated() {return _activated;}

		/**
		Devuelve los atributos de la entidad en forma de
		Map::CEntity
		*/
		const Map::CEntity* getEntityInfo() const { return _entityInfo; }

		/**
		Indica si la unidad es un generador.

		@return true si la unidad es un generador.
		*/
		bool isGenerator() {return _isGenerator;}

		/**
		Indica si la unidad es un enemigo.

		@return true si la unidad es un enemigo.
		*/
		bool isEnemy() {return _isEnemy;}

	protected:

		/**
		Clase amiga que puede modificar los atributos (en concreto se 
		usa para modificar el mapa.
		*/
		friend class CMap;

		/**
		Identificador único de la entidad.
		*/
		Logic::TEntityID _entityID;

		/**
		Tipo para la lista de componetes.
		*/
		typedef std::list<IComponent*> TComponentList;

		// Enitty's active components
		TComponentList activeComponents;

		// Entity's deactive components 
		TComponentList deactiveComponents;

		/**
		Indica si la entidad está activa.
		*/
		bool _activated;

		/**
		Tipo de la entidad declarado en el archivo blueprints.
		*/
		std::string _type;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Mapa lógico donde está la entidad.
		*/
		Logic::CMap *_map;

		/**
		Matriz de transformación de la entidad. Contiene posición y orientación.
		*/
		Matrix4 _transform;

		/**
		Atributo que indica si la entidad es una entidad usada para
		instanciarla múltiples veces. Por defecto es false.
		*/
		bool _isGenerator;

		/*
		Posición de la entidad.
		*
		Vector3 _position;

		/*
		Posición de la entidad.
		*
		float _orientation;

		/**
		Atributo que indica si la entidad es el jugador; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isPlayer;

		/**
		Atributo que indica si la entidad es un enemigo; por defecto
		es false a no ser que se lea otra cosa de los atributos.
		*/
		bool _isEnemy;

		/**
		Información de los atributos base de la entidad
		*/
		const Map::CEntity* _entityInfo;

		/**
			Searches a component by type on the given list.

			@param type The componont type that is wanted to find.
			@param componentList The list where to look for the component.

			@return A pointer to the component or nullptr if it is not found.
		*/
		IComponent* searchComponentByType(std::string type, const TComponentList &componentList);

		/**
			Removes the component on the given list if the component is on it.

			@param component The component that is wanted to remove.
			@param componentList The list where to look for the component.

			@return True if the component has been removed from the list.
		*/
		bool CEntity::removeComponent(IComponent* component, TComponentList &componentList);

	}; // class CEntity

} // namespace Logic

#endif // __Logic_Entity_H
