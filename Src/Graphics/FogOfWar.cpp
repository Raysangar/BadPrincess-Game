#include "FogOfWar.h"

#include "OgreSubEntity.h"

namespace Graphics
{
	void CFogOfWar::addUnit(Logic::CEntity* unit, VisionAttributes visionAttributes)
	{
		int unitIndex = getUnitIndex(nullptr);
		registeredUnits[unitIndex] = unit;
		unitAttributes[unitIndex] = visionAttributes;
		updateUnitShaderAttributes(unitIndex, Vector4(unitAttributes[unitIndex].first.x, unitAttributes[unitIndex].first.y, 
			unitAttributes[unitIndex].first.z, unitAttributes[unitIndex].second));
	}

	//---------------------------------------------------------

	void CFogOfWar::removeUnit(Logic::CEntity* unit)
	{
		int unitIndex = getUnitIndex(unit);
		unitAttributes.erase(unitIndex);
		registeredUnits[unitIndex] = nullptr;
		updateUnitShaderAttributes(unitIndex, Vector4(0, 0, 0, 0));
	}

	//---------------------------------------------------------

	void CFogOfWar::updateUnitPosition(Logic::CEntity* unit, Vector3 position)
	{
		int unitIndex = getUnitIndex(unit);
		unitAttributes[unitIndex].first = position;
		updateUnitShaderAttributes(unitIndex, Vector4(position.x, position.y, position.z, unitAttributes[unitIndex].second));
	}

	void CFogOfWar::updateUnitShaderAttributes(int unitIndex, Vector4 shaderAttribute)
	{
		for (Ogre::SubEntity* fogOfWarSubEntity : fogOfWarSubEntities)
			fogOfWarSubEntity->setCustomParameter(unitIndex, shaderAttribute);
	}

	//---------------------------------------------------------

	int CFogOfWar::getUnitIndex(Logic::CEntity* unit)
	{
		int unitIndex = -1;
		while (registeredUnits[++unitIndex] != unit){}
		return unitIndex;
	}

	//---------------------------------------------------------

	void CFogOfWar::addFogOfWarSubEntity(Ogre::SubEntity* fogOfWarSubEntity)
	{
		fogOfWarSubEntities.push_back(fogOfWarSubEntity);
		std::list<ShaderAttribute> shaderAttributes;
		updateShaderParameters(&shaderAttributes);
		updateSubEntityParameters(fogOfWarSubEntity, &shaderAttributes);
	}

	//---------------------------------------------------------

	void CFogOfWar::removeFogOfWarSubEntity(Ogre::SubEntity* fogOfWarSubEntity)
	{
		fogOfWarSubEntities.remove(fogOfWarSubEntity);
	}

	//---------------------------------------------------------

	void CFogOfWar::updateAll()
	{
		std::list<ShaderAttribute> shaderAttributes;
		updateShaderParameters(&shaderAttributes);
		for (Ogre::SubEntity* fogOfWarSubEntity : fogOfWarSubEntities)
			updateSubEntityParameters(fogOfWarSubEntity, &shaderAttributes);
	}

	//---------------------------------------------------------

	void CFogOfWar::updateShaderParameters(std::list<ShaderAttribute>* shaderParameters)
	{
		std::map<int, VisionAttributes>::iterator unitAttributesIterator = unitAttributes.begin();

		for (; unitAttributesIterator != unitAttributes.end(); ++unitAttributesIterator)
			insertAttributesOnList(shaderParameters, &unitAttributesIterator);
	}

	//---------------------------------------------------------

	void CFogOfWar::updateSubEntityParameters(Ogre::SubEntity* subEntity, std::list<ShaderAttribute>* shaderAttributes)
	{
		for (std::pair<int, Vector4> shaderAttribute : *shaderAttributes)
			subEntity->setCustomParameter(shaderAttribute.first, shaderAttribute.second);
	}

	//---------------------------------------------------------

	void CFogOfWar::insertAttributesOnList(std::list<ShaderAttribute>* shaderParameters, UnitAttributes::iterator* unitAttributesIterator)
	{
		Vector3 position = (*unitAttributesIterator != unitAttributes.end()) ?
			(*unitAttributesIterator)->second.first : Vector3::ZERO;

		Vector4 shaderParameter = Vector4(position.x, position.y, position.z, (*unitAttributesIterator != unitAttributes.end()) ?
			(*unitAttributesIterator)->second.second : 0.0f);

		shaderParameters->push_back(ShaderAttribute((*unitAttributesIterator)->first, shaderParameter));
	}

	//---------------------------------------------------------

	void CFogOfWar::restart()
	{
		fogOfWarSubEntities.clear();
		for (int i = 0; i < MAXIMUN_NUMBER_OF_UNITS; ++i)
			registeredUnits[i] = nullptr;
		unitAttributes.clear();
	}

} //namespace Graphics