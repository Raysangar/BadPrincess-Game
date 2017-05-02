#ifndef __Graphics_FogOfWar_H
#define __Graphics_FogOfWar_H

#include "BaseSubsystems/Math.h"
#include "OgreCommon.h"
#include "OgrePrerequisites.h"
#include "OgreGpuProgramParams.h"

#define MAXIMUN_NUMBER_OF_UNITS 24

namespace Ogre 
{
	class Material;
}

namespace Logic
{
	class CEntity;
}

namespace Graphics 
{
	class CFogOfWar 
	{
	public:
		typedef std::pair<Vector3, float>  VisionAttributes;

		CFogOfWar()
		{
			for (int i = 0; i < MAXIMUN_NUMBER_OF_UNITS; ++i)
				registeredUnits[i] = nullptr;
		}

		virtual ~CFogOfWar(){}

		void addUnit(Logic::CEntity* unit, VisionAttributes visionAttributes);

		void removeUnit(Logic::CEntity*);

		void updateUnitPosition(Logic::CEntity* unit, Vector3 position);

		void addFogOfWarSubEntity(Ogre::SubEntity* fogOfWarMaterial);

		void removeFogOfWarSubEntity(Ogre::SubEntity* fogOfWarMaterial);

		void updateAll();

		void restart();

	private:

		std::list<Ogre::SubEntity*> fogOfWarSubEntities;

		typedef std::map<int, VisionAttributes> UnitAttributes;

		Logic::CEntity* registeredUnits[MAXIMUN_NUMBER_OF_UNITS];

		typedef std::pair<int, Vector4> ShaderAttribute;

		// Map with current position and vision range from ally units
		UnitAttributes unitAttributes;

		int getUnitIndex(Logic::CEntity*);

		void updateUnitShaderAttributes(int unitIndex, Vector4 shaderAttribute);

		void updateShaderParameters(std::list<ShaderAttribute>* shaderParameters);

		void updateSubEntityParameters(Ogre::SubEntity* subEntity, std::list<ShaderAttribute>* shaderAttributes);

		void insertAttributesOnList(std::list<ShaderAttribute>* shaderParameters, UnitAttributes::iterator* unitAttributesIterator);
	};
}

#endif
