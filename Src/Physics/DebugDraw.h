#ifndef _PHYSICS_DEBUGDRAW_H_
#define _PHYSICS_DEBUGDRAW_H_

namespace Ogre
{
	class Vector3;
}

void compileColors();
void drawLine(const Ogre::Vector3& initPos, const Ogre::Vector3& endPos, char color);
void drawTriangle(Ogre::Vector3 firstPos, Ogre::Vector3 secondPos, Ogre::Vector3 thirdPos);
void removeLines();

#endif