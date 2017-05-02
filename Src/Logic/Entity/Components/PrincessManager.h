/**
@file PrincessManager.h

Contiene la declaración del componente que controla la representación
el estado de selección de una entidad controlable.

@see Logic::CGraphics
@see Logic::IComponent

@author Iván León
@date Enero, 2015
*/
#ifndef __Logic_PrincessManager_H
#define __Logic_PrincessManager_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CFrustumEntity;
	class CScene;
}

enum WrathModifiers
{
	attack = 0,
	faint = 1,
	casualty = 2,
	damaged = 3,
	aoeHeal = 4,
	aoeBuff = 5,
	aoeStun = 6,
	berserk = 7
};

//declaración de la clase
namespace Logic 
{
	class CCamera;
/**
	Componente que se encarga de gestionar si una entidad controlable está seleccionada 
	para recibir acciones o no. En caso positivo, dibuja un círculo bajo la misma.
	
    @ingroup logicGroup

	@author Iván León
	@date Enero, 2015
*/
	class CPrincessManager : public IComponent
	{
		DEC_FACTORY(CPrincessManager);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CPrincessManager(std::string componentName) : IComponent(componentName) , _materialToProject(""),_node(nullptr),
			_size(Vector2::ZERO),_projectionHeight(0), _scene(nullptr),_wrath(0), _wrathPerCasualty(10), _wrathPerDamage(2),_timer(0),_raptor(0),
			_limitTimeOnDangerousWrathLevel(0), _currentTimeOnDangerousWrathLevel(0),_isUnconscius(false), wrathOverflowParticleName("blackHole"),_wrathOverflowActive(true)
		{
		}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CPrincessManager();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el tamaño de la 
		cápsula que representa la entidad física y usa su radio para asignárselo
		al círculo de selección.
		Después crea un ManualObject para dibujar el círculo y lo asigna a un
		SceneNode de Ogre también recién creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posición.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		const int getTimeToAwake() { return _timeToAwake; }

		Logic::CEntity* getRaptor() { return _raptor; }

		void setRaptor(Logic::CEntity* raptor) { _raptor = raptor; }

		int  getAoeBuffCost() const { return _aoeBuffCost; }
		int  getAoeHealCost() const { return _aoeHealCost; }
		int  getAoeStunCost() const { return _aoeStunCost; }
		int  getBerserkerModeCost() const { return _berserkerModeCost; }

		float getWrathAreaRadius();

		void setIsUnconscius(bool isUnconscius) { _isUnconscius = isUnconscius; }

		void setWrathOverflowActive(bool active) { _wrathOverflowActive = active; }
		
	protected:

		void initDecal();

		void updateSize(Ogre::Vector2 size);

		void modifyWrath(int reason);

		void aoeHealing();

		void aoeBuff();

		void wrathOverflow();

		/**
		Habilidad para aturdir enemigos en area.
		*/
		void aoeStun();

		void activateBerserkerMode();
		
		/**
		Variables para la habilidad de mejora de aliados en area
		*/
		int _aoeBuffDuration;
		float _aoeBuffPercentage; 
		int _aoeBuffCost;
		std::string _aoeBuffParticle;

		/**
		Variables para la habilidad de sanación en area
		*/
		float _aoeHealQuantity; 
		int _aoeHealCost;
		std::string _aoeHealParticle;

		/**
		Variables de la habilidad para aturdir enemigos en area.
		*/
		float _aoeStunTime;
		float _aoeStunDistance; 
		int _aoeStunCost;
		std::string _aoeStunParticle;

		// Berserker mode variables
		int _berserkerModeCost;
		unsigned int _berserkerModeDuration;
		float _berserkerModeAttackModifier;
		bool _berserkerModeActivated;
		unsigned int _berserkerModeTimer;
		std::string _berserkerParticle;

		//Wrath overflow variables
		int _limitTimeOnDangerousWrathLevel;
		int _currentTimeOnDangerousWrathLevel;
		int _wrathOverflowDamage;
		std::string wrathOverflowParticleName;
		std::string wrathOverflowSoundName;

		bool _isUnconscius;

		/**
		Medidor de ira de la princesa
		*/
		int _wrath;

		/**
		Variables referentes a la ira de la princesa
		*/
		int _wrathPerCasualty;
		int _wrathPerDamage;
		int _wrathPerAttack;
		float _timeToDecreaseWrath;
		float _timer;
		

		/**
		Tiempo necesario de la princesa para reanimarse tras caer en combate y permanecer en el suelo
		*/
		int _timeToAwake;

		/**
		Referencia al caballero que está raptando a la princesa
		*/
		Logic::CEntity* _raptor;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la creación y destrucción del círculo.
		*/
		Graphics::CScene* _scene;

		/**
		Variables para la proyección del aura
		*/
		float _projectionHeight;

		float _heightIncrease;

		std::string _materialToProject;
 
		Graphics::CFrustumEntity *_node;
 
	    void addMaterial(std::string matName);

		Vector2 _size;        // size of decal

		bool _wrathOverflowActive;

	}; // class CGraphics

	REG_FACTORY(CPrincessManager);

} // namespace Logic

#endif // __Logic_Graphics_H
