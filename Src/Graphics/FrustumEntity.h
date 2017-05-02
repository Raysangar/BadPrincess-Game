//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

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
		Destructor de la aplicaci�n.
		*/
		virtual ~CFrustumEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
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
		Ogre::Frustum *_entity;
	
	}; // class CFrustumEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
