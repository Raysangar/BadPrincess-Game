/**
@file ParticleEntity.h

Contains the declaration of the particle entity class.

@see Graphics::CEntity

@author Rayco Sánchez García
@date March, 2015
*/

#ifndef __Graphics_FrustumEntity_H
#define __Graphics_FrustumEntity_H

#include "Graphics\Entity.h"

namespace Ogre
{
	class ParticleSystem;
	class ParticleEmitter;
}

namespace ParticleUniverse
{
	class ParticleSystem;
}

namespace Graphics 
{
	class CScene;

	class CParticleEntity : public CEntity
	{
	public:

		CParticleEntity(const std::string &name, const std::string &particleSystemPath);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CParticleEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		virtual void setScale(float scale);

		void start();

		void pause();

		void stop();

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
			
		// The particle system
		ParticleUniverse::ParticleSystem* _particleSystem;

		std::string _particleSystemName;
	}; // class CFrustumEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
