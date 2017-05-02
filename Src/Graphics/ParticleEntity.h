/**
@file ParticleEntity.h

Contains the declaration of the particle entity class.

@see Graphics::CEntity

@author Rayco S�nchez Garc�a
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
		Destructor de la aplicaci�n.
		*/
		virtual ~CParticleEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		virtual void setScale(float scale);

		void start();

		void pause();

		void stop();

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
			
		// The particle system
		ParticleUniverse::ParticleSystem* _particleSystem;

		std::string _particleSystemName;
	}; // class CFrustumEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
