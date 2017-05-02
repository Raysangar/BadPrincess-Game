//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CMeshEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_MeshEntity_H
#define __Graphics_MeshEntity_H

#include "Graphics\Entity.h"

namespace Ogre 
{
	class Entity;
	class SubEntity;
	
}
namespace Graphics
{
	class CFrustumEntity;
}
namespace Graphics 
{
	/**
	Clase que representa una entidad gráfica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gráfico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, ésta debe añadirse a una escenada de Ogre mediante el 
	método attachToScene(). Solo la clase CScene puede invocar los métodos 
	para añadir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al añadir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gráfica (posición, orientación, escala...) se deberán hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características. Por ejemplo, ésta clase
	no permite especificar la animación utilizada. Si queremos que podamos
	guardar un modelo gráfico, y poder cambiar la animación actual,
	se utilizará la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CMeshEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CMeshEntity(const std::string &name, const std::string &mesh);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CMeshEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		Ogre::Entity* getEntity() {return _entity;}
		/**
		Create an instance of the material and set the tranparency into an specific tecnique,pass and texture unit.

		@param name name of the material.
		@param tecnique position of the tecnique.
		@param pass position of the pass.
		@param unitState position of the unitState.
		@param tranparency value of tranparency [0,1] where 0 is completely transparent and 1 is opaque.
		*/
		void setMaterialTransparency(std::string name,int tecnique,int pass,int unitState,float tranparency);

		/**
		Create an instance of the material and set the tranparency into an specific tecnique,pass and texture unit.

		@param name name of the material.
		@param tecnique position of the tecnique.
		@param pass position of the pass.
		@param unitState position of the unitState.
		@param tranparency value of tranparency [0,1] where 0 is completely transparent and 1 is opaque.
		*/
		void setMaterialTransparency(std::vector<std::string> name,int tecnique,int pass,int unitState,float tranparency);

		/**
		Apply a new material to the entire _entity

		@param name name of the material.
		*/
		void changeEntityMaterial(std::string name);

		/**
		Apply a new material to a specific subentity

		@param name name of the material.
		*/		
		void changeSubEntityMaterial(Ogre::SubEntity *sub, std::string name);

		void projectTexture(std::string destinationMaterialName,int technique,Graphics::CFrustumEntity * frustum,std::string materialToProject, std::string sourceEntityName);

		std::string getMeshName() { return _mesh; }

		void registerSubEntitiesOnFogOfWarController();

		void unregisterSubEntitiesOnFogOfWarController();

		std::list<Ogre::SubEntity*> getSubEntities();

		std::vector<Vector3> getMeshCorners();

		void addTexture(std::string textureName);

		void removePass(int passIndex);
		/** 
		Attaches another object to a certain bone of the skeleton which this entity uses.
		  @par
		  An exception is thrown if the movable object is already attached to the bone, another bone or scenenode.
		  If the entity has no skeleton or the bone name cannot be found then an exception is thrown.
		  @param boneName The name of the bone (in the skeleton) to attach this object
		  @param obj Pointer to the mesh entity to attach
		  @param offsetOrientation An adjustment to the orientation of the attached object, relative to the bone.
		  @param offsetPosition An adjustment to the position of the attached object, relative to the bone.		  
		  */
		void attachObjectToBone(std::string boneName,CMeshEntity* obj,const Quaternion &offsetOrientation = Quaternion::IDENTITY,const Vector3 &offsetPosition = Vector3::ZERO);

		void detachObjectFromBone(CMeshEntity* obj);

		bool instantiateEntity(CScene*scene);

		void setAttachedToBone(bool isAttached){_attached = isAttached;};

		bool isAttached(){return _attached;};
	protected:

		// CScene es la única que puede añadir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

				
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);
			
		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;


		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;

		// check if the entity is already attached to the scene
		bool _attached;

	
	}; // class CMeshEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
