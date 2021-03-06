//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica con Manual Object.

@see Graphics::CManualEntity

@author Guidi Giacomo
@date Julio, 2010
*/

#ifndef __Graphics_ManualEntity_H
#define __Graphics_ManualEntity_H

#include "Graphics\Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class ManualObject;	
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gr�fica Manual. Contiene una referencia a
	la entidad ManualObject de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escenada de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al a�adir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gr�fica (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas. Por ejemplo, �sta clase
	no permite especificar la animaci�n utilizada. Si queremos que podamos
	guardar un modelo gr�fico, y poder cambiar la animaci�n actual,
	se utilizar� la clase derivada correspondiente.
	
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
		Destructor de la aplicaci�n.
		*/
		virtual ~CManualEntity();

		
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
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

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;


		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
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
