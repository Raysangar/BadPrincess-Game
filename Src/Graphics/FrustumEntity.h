//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CFrustumEntity

@author Guidi Giacomo
@date March, 2015
*/

#ifndef __Graphics_FrustumEntity_H
#define __Graphics_FrustumEntity_H

#include "Graphics\Entity.h"
#include <OgreFrustum.h>


namespace Graphics 
{


	class CFrustumEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CFrustumEntity(const std::string &name);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CFrustumEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		void setAspectRatio(float aspectRatio);

		void setOrthoWindowHeight(float height);

		void setDebugDisplayEnabled(bool value);

		void setProjectionType(Ogre::ProjectionType type);

		void CFrustumEntity::setFarClipDistance(float height);

		const Ogre::Frustum * getFrustum(){return _entity;};

	protected:

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
		Ogre::Frustum *_entity;
	
	}; // class CFrustumEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
