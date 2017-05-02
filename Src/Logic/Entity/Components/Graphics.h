/**
@file Graphics.h

Contiene la declaración del componente que controla la representación
gráfica de la entidad.

@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Graphics_H
#define __Logic_Graphics_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CMeshEntity;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representación gráfica de una entidad.
	En concreto se encarga de las entidades con representaciones gráficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan éste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de cambio de posición y orientación (matriz de 
	transformación) mediante el mensaje SET_TRANSFORM.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CGraphics : public IComponent
	{
		DEC_FACTORY(CGraphics);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CGraphics(std::string componentName) : IComponent(componentName), _graphicsEntity(0), _model(""),_currentModel(""),
			_scene(nullptr), isStatic(false), _scale(1.0),_currentScale(1.0f),_isActive(false),_gradualTransparency(false),_currentTransparency(1.0f) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CGraphics();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deberá cargar e invoca al método virtual
		createGraphicsEntity() para generar la entidad gráfica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual bool activateGraphics();

		virtual void deactivate();

		virtual void awake();

		virtual void restart();

		virtual void pause();

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.
		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		virtual void tick(unsigned int msecs);

		/**
		Create an instance of the material and set the tranparency into an specific tecnique,pass and texture unit.

		@param name name of the material.
		@param tecnique position of the tecnique.
		@param pass position of the pass.
		@param unitState position of the textureUnitState.
		@param tranparency value of tranparency [0,1] where 0 is completely transparent and 1 is opaque.
		*/
		void setMaterialTransparency(std::string name,int tecnique,int pass,int unitState,float tranparency, bool gradual = false, int gradualMsecs = 0);

		/**
		Create an instance of the material and set the tranparency into an specific tecnique,pass and texture unit.

		@param name name of the material.
		@param tecnique position of the tecnique.
		@param pass position of the pass.
		@param unitState position of the textureUnitState.
		@param tranparency value of tranparency [0,1] where 0 is completely transparent and 1 is opaque.
		*/
		void setMaterialTransparency(std::vector<std::string> name,int tecnique,int pass,int unitState,float tranparency, bool gradual = false, int gradualMsecs = 0);

		/**
		Change the entity's graphic model if it isn't static.

		@param name name of the mesh model.
		@param scale
		*/
		void changeModel(std::string name,float scale);

		Graphics::CMeshEntity* getGraphicsEntity(){return _graphicsEntity;};

		virtual void setVisible(bool isVisible);

	protected:

		/**
		Método virtual que construye la entidad gráfica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gráficas (animadas, etc.).
		
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Entidad gráfica creada, NULL si hubo algún problema.
		*/
		virtual Graphics::CMeshEntity* createGraphicsEntity(const Map::CEntity *entityInfo);

		/**
		Atributo con el nombre del modelo gráfico de la entidad.
		*/
		std::string _model;
		
		std::string _currentModel;

		/**
		Flag para establecer que el cambio de transparencia es gradual
		*/
		bool _gradualTransparency;

		/**
		Para almacenar el máximo valor de transparencia a alcanzar
		*/
		float _maxGradualTransparency;
		float _currentTransparency;

		/**
		Variables para la transparencia gradual
		*/
		std::string _materialName;
		std::vector<std::string> _materialNames;

		int _technique;
		int _pass;
		int _textureUnit;		

		/**
		Para almacenar el valor de transparencia que tiene que ser modificado por msecs transcurridos en el tick
		*/
		float _transparencyPerMsec;

		/**
		Entidad gráfica.
		*/
		Graphics::CMeshEntity *_graphicsEntity;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		Graphics::CScene* _scene;

		bool isStatic;

		float _scale;

		float _currentScale;

		
		/**
		Flag used to mark whether the componet has been already actived;
		*/
		bool _isActive;


		/**
		@author Rayco Sánchez García
		@date January, 2015

		Function that sets scale and transform attributes to graphics entity.

		@param entityInfo Entity construction info.
		*/
		virtual void setGraphicsEntityAttributes();
	}; // class CGraphics

	REG_FACTORY(CGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
