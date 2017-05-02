/**
@file BillboardSetEntity.h

Contains the declaration of the billboard set class.

@see Graphics::CEntity

@author Guidi Giacomo
@date March, 2015
*/

#ifndef __Graphics_BillboardSetEntity_H
#define __Graphics_BillboardSetEntity_H

#include "Graphics\Entity.h"

namespace Ogre
{
	class BillboardSet;
	class Billboard;
}

namespace Graphics 
{
	class CScene;
	/*Correspond exactly to Ogre::BillboardOrigin
	Enum covering what exactly a billboard's position mean
	*/
	enum BillboardSetOrigin{
		BBO_TOP_LEFT,
		BBO_TOP_CENTER,
		BBO_TOP_RIGHT,
		BBO_CENTER_LEFT,
		BBO_CENTER,
		BBO_CENTER_RIGHT,
		BBO_BOTTOM_LEFT,
		BBO_BOTTOM_CENTER,
		BBO_BOTTOM_RIGHT 
	};
	/*Correspond exactly to Ogre::BillboardType
	The type of billboard to use.
	It's relative to their orientation.
	*/
	enum BillboardType{
		BBT_POINT,
		BBT_ORIENTED_COMMON,
		BBT_ORIENTED_SELF,
		BBT_PERPENDICULAR_COMMON,
		BBT_PERPENDICULAR_SELF 
	};

	class CBillboardSetEntity : public CEntity
	{
	public:

		CBillboardSetEntity(const std::string &name,BillboardType type = BillboardType::BBT_POINT,unsigned int poolSize = 20);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CBillboardSetEntity();

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		void addBillboard(Vector3 position,Vector2 dimension,float r,float g,float b);

		void setBilboardColor(unsigned int billboardIdx,float r,float g,float b);

		void setBilboardDimension(unsigned int billboardIdx,float w,float h);

		void setBilboardPosition(unsigned int billboardIdx,Vector3 pos);

		void setBilboardTexCoordRect(unsigned int billboardIdx,float p0x,float p0y,float p1x,float p1y);

		void setBilboardSetOrigin(BillboardSetOrigin origin);

		void setMaterial(std::string materialName);

		virtual void setVisible(bool visible);

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
		Ogre::BillboardSet* _billboardSet;

		unsigned int _poolSize;

		BillboardType _type;

	}; // class CFrustumEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
