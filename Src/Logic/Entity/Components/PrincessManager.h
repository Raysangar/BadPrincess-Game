/**
@file PrincessManager.h

Contiene la declaraci�n del componente que controla la representaci�n
el estado de selecci�n de una entidad controlable.

@see Logic::CGraphics
@see Logic::IComponent

@author Iv�n Le�n
@date Enero, 2015
*/
#ifndef __Logic_PrincessManager_H
#define __Logic_PrincessManager_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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

//declaraci�n de la clase
namespace Logic 
{
	class CCamera;
/**
	Componente que se encarga de gestionar si una entidad controlable est� seleccionada 
	para recibir acciones o no. En caso positivo, dibuja un c�rculo bajo la misma.
	
    @ingroup logicGroup

	@author Iv�n Le�n
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CPrincessManager();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el tama�o de la 
		c�psula que representa la entidad f�sica y usa su radio para asign�rselo
		al c�rculo de selecci�n.
		Despu�s crea un ManualObject para dibujar el c�rculo y lo asigna a un
		SceneNode de Ogre tambi�n reci�n creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		@author Rayco S�nchez Garc�a
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posici�n.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

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
		Variables para la habilidad de sanaci�n en area
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
		Referencia al caballero que est� raptando a la princesa
		*/
		Logic::CEntity* _raptor;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la creaci�n y destrucci�n del c�rculo.
		*/
		Graphics::CScene* _scene;

		/**
		Variables para la proyecci�n del aura
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
