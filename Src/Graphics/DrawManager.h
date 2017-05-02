//---------------------------------------------------------------------------
// DrawManager.h
//---------------------------------------------------------------------------

#ifndef __Graphics_DrawManager_H
#define __Graphics_DrawManager_H

#include <string>
#include <list>
#include <OgreRenderOperation.h>
#include "BaseSubsystems\Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Vector3;	
	class ManualObject;
	class SceneNode;

}
namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{

	class CDrawManager 
	{
	public:

		/**
		Constructor de la clase.

		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CDrawManager(CScene *scene);
		
		/**
		Destructor de la aplicación.
		*/
		virtual ~CDrawManager();		

		/**
		Crea y compila materiales para dar color a los tres ejes.

		*/
		void compileColor(std::string name, float red, float green, float blue, float alpha);

		/**
		Dibuja una linea entre las posiciones indicadas

		@param initPos Punto de origen de la linea
		@param endPos Punto final de la linea
		@param sceneNode Objeto del tipo SceneNode donde el manual object irá atachado
		@param color Puede ser 'w' (blanco), 'r'(rojo), 'g'(verde), 'b'(azul)
		@param name Nombre que se asignará al manualObject
		*/
		void drawLine(const Ogre::Vector3& initPos, const Ogre::Vector3& endPos, Ogre::SceneNode* sceneNode, char color = 'w', std::string name = "", bool dynamic = false);

		void drawLine(const Ogre::Vector3& initPos, const Ogre::Vector3& endPos, Ogre::ManualObject* line, char color='w');
		
		/**
		Dibuja un ManualObject entre las posiciones indicadas y lo asigna al SceneNode indicado.
		Antes de crear un nuevo ManualObject comprueba que no existe previamente. En ese caso crea
		uno nuevo con el nombre indicado y pinta con begin(). 
		En caso de que el ManualObject ya exista, con beginUpdate() modifica el dibujo con 
		las nuevas posiciones.
		Tras esto, asigna el manualObject al SceneNode indicado.

		@param firstPos Primer punto del rectángulo
		@param secondPos Segundo punto del rectángulo
		@param thirdPos Tercer punto del rectángulo
		@param fourthPos Cuarto punto del rectángulo
		@param sceneNode Objeto del tipo SceneNode donde el manual object irá atachado
		@param color Puede ser 'w' (blanco), 'r'(rojo), 'g'(verde), 'b'(azul)
		@param name Nombre que se asignará al manualObject
		*/
		void drawRectangle(const Ogre::Vector3& firstPos, const Ogre::Vector3& secondPos, const Ogre::Vector3& thirdPos, const Ogre::Vector3& fourthPos,
			Ogre::SceneNode* sceneNode, char color = 'w', std::string name = "", bool dynamic = false);

		/*
			Function that draw a circle with segment in XZ axes

			@param circle The Ogre item used to draw the shape
			@param center The center point of the shape to draw
			@param radius the radius of the circle
			@param accuracy The quantity of sample
			@param startAngle the radiant angle where it compute the first point
			@param endAngle the radiant angle where it compute the last point
		*/
		void drawCircleShapeXZ(Ogre::ManualObject * circle,const Ogre::Vector3 center,const float radius,const float accuracy = 20, const float startAngle = 0, const float endAngle = 2 * Math::PI);
		/*
			Function that draw a circle with segment in XY axes with a certain rotation in Z axe

			@param circle The Ogre item used to draw the shape
			@param center The center point of the shape to draw
			@param radius the radius of the circle
			@param accuracy The quantity of sample
			@param startAngle the radiant angle where it compute the first point
			@param endAngle the radiant angle where it compute the last point
		*/
		void drawCircleShapeXY(Ogre::ManualObject * circle,const Ogre::Vector3 center,const float zRotation,const float radius,const float accuracy = 20, const float startAngle = 0, const float endAngle = 2 * Math::PI);
		/*
			Function that draw a cilinder perpendicular to XZ axes

			@param cilinder The Ogre item used to draw the shape
			@param center The center point of the shape to draw
			@param halfHeight the half height of cilinder
			@param radius the radius of cilinder
			@param accuracy The quantity of sample
		*/
		void drawCilinderShapeXZ(Ogre::ManualObject * cilinder,const Ogre::Vector3 center,const float halfHeight,const float radius,const float accuracy);

		/*
			Function that draw an unitary 3D box

			@param cube The Ogre item used to draw the shape
			
		*/
		void drawUnitaryBoxShape(Ogre::ManualObject * cube);
		/*
			Function that draw a capsule

			@param capsule The Ogre item used to draw the shape			
			@param halfHeight the half height of cilinder
			@param radius the radius of cilinder
		*/
		void drawCapsuleShape(Ogre::ManualObject * capsule,const float halfHeight,const float radius);
		/**
		Dibuja una linea OT_LINE_STRIP entre las posiciones en la lista indicadas

		@param list lista de punto
		@param sceneNode Objeto del tipo SceneNode donde el manual object irá atachado
		@param color Puede ser 'w' (blanco), 'r'(rojo), 'g'(verde), 'b'(azul)
		@param name Nombre que se asignará al manualObject
		*/
		void drawLine(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic);
		/**
		Dibuja una lista de punto 

		@param list lista de punto
		@param sceneNode Objeto del tipo SceneNode donde el manual object irá atachado
		@param color Puede ser 'w' (blanco), 'r'(rojo), 'g'(verde), 'b'(azul)
		@param name Nombre que se asignará al manualObject
		*/

		void drawPointList(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic);
		
		/**
		Dibuja una linea OT_LINE_LIST entre las posiciones en la lista indicadas

		@param list lista de punto
		@param sceneNode Objeto del tipo SceneNode donde el manual object irá atachado
		@param color Puede ser 'w' (blanco), 'r'(rojo), 'g'(verde), 'b'(azul)
		@param name Nombre que se asignará al manualObject
		*/
		void drawSegmentList(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic);

		void setColor(Ogre::ManualObject * m,char color,Ogre::RenderOperation::OperationType op);

		
	protected:

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;


		///**
		//Indica el índice de cada nuevo manualObject creado
		//*/
		//int _manualCount;

	}; // class DrawManager

} // namespace Graphics

#endif // __Graphics_DrawManager_H
