/**
@file StatsModifier.h

Contiene la implementaci�n del componente que gestiona
los atributos �nicos en el Goblin Montaraz

@see Logic::IComponent

@author Iv�n Le�n
@date Febrero, 2015
*/

#ifndef __Logic_StatsModifier_H
#define __Logic_StatsModifier_H

#include "Logic/Entity/Component.h"


namespace Ogre
{
	class Vector3;
}

namespace Logic 
{
	class CEntity;	
	class CLife;
	class CCombatStats;
	class CSteeringMovement;

	enum ModificationType
	{
		attackMod = 0,
		lifeMod = 1,
		speedMod = 2,
		berserker =3
	};

	struct StatModification
	{
		ModificationType type;		
		float percentage;		
		unsigned int endingTime;
		unsigned int acumTime;
		bool restore;
	};

	class CStatsModifier : public IComponent
	{

	DEC_FACTORY(CStatsModifier);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CStatsModifier(std::string componentName) : IComponent(componentName), _steeringComponent(0), _lifeComponent(0), _combatComponent(0),_originalAttackDamage(0),
			_originalLife(0),_originalMovementSpeed(0)
		{
			modificationHandlers[ModificationType::attackMod] = &CStatsModifier::modifyAttack;
			modificationHandlers[ModificationType::lifeMod] = &CStatsModifier::modifyLife;
			modificationHandlers[ModificationType::speedMod] = &CStatsModifier::modifySpeed;
			modificationHandlers[ModificationType::berserker] = &CStatsModifier::setBerserkerMode;
		}

		/**
		Destructor (virtual); 
		*/
		virtual ~CStatsModifier();

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		EnemySelected.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);


		/**
		M�todo que se ejecuta tras cargar todos los componentes
		*/
		virtual void awake();
		

		/**
		En el tick comprobaremos la lista de modificaciones. 
		Si la modificaci�n tiene una duraci�n mayor de 0, comprobamos si se ha cumplido dicha duraci�n, 
		en cuyo caso restauramos los valores iniciales, eliminamos la modificaci�n y la sacamos de la lista.
		*/
		virtual void tick(unsigned int msecs);


	protected:

		typedef void (CStatsModifier::*processModification)(StatModification* mod);

		std::map<int,  processModification> modificationHandlers;

		/**
		Funciones para modificar los diferentes tipos de estados
		*/
		void modifyLife(StatModification* mod);
		void modifyAttack(StatModification* mod);
		void modifySpeed(StatModification* mod);
		void setBerserkerMode(StatModification* mod);
		
		/**
		Variables para almacenar las referencias a los componentes
		*/
		Logic::CLife* _lifeComponent;
		Logic::CCombatStats* _combatComponent;
		Logic::CSteeringMovement* _steeringComponent;

		std::map<std::string,StatModification*> modifications;	

		float _originalLife;
		int _originalAttackDamage;
		float _originalMovementSpeed;

	}; 

	REG_FACTORY(CStatsModifier);
} 

#endif
