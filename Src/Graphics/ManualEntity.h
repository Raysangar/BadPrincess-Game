//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica con Manual Object.

@see Graphics::CManualEntity

@author Guidi Giacomo
@date Julio, 2010
*/

#ifndef __Graphics_ManualEntity_H
#define __Graphics_ManualEntity_H

#include "Graphics\Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class ManualObject;	
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gráfica Manual. Contiene una referencia a
	la entidad ManualObject de Ogre y al Nodo que la contiene.
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

	@author Guidi Giacomo
	@date Febrero, 2015
	*/
	class CManualEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CManualEntity(const std::string &name);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CManualEntity();

		
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		Devuelve la entidad de Ogre.

		@return puntero a la entidad de Ogre.
		*/
		Ogre::ManualObject* getEntity() {return _entity;};
		/*
			Function that draw an unitary 3D box
			
		*/
		void drawUnitaryBoxShape();
		/*
			Function that draw a capsule

			@param halfHeight the half height of cilinder
			@param radius the radius of cilinder
		*/
		void drawCapsuleShape(const float halfHeight,const float radius);

		void drawCircleXZ(const float radius);

		void drawSegment(Vector3 start,Vector3 end,char color='w');

		bool isEmpty(){return _isEmpty;};
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
		Ogre::ManualObject *_entity;

		/*
			Indica si hemos ya dibujato algo.
		*/
		bool _isEmpty;

	}; // class CManualEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
