/**
@file StatsModifier.cpp
*/
#include "StatsModifier.h"
#include "CombatStats.h"
#include "Life.h"
#include "SteeringMovement.h"

#include "Application/BaseApplication.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/UnitMessages.h"

namespace Logic 
{
	IMP_FACTORY(CStatsModifier);

	//---------------------------------------------------------	

	bool CStatsModifier::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;
	} 

	//---------------------------------------------------------

	void CStatsModifier::awake()
	{		
		_lifeComponent = static_cast<Logic::CLife*>(_entity->getComponent("CLife"));
		_steeringComponent = static_cast<Logic::CSteeringMovement*>(_entity->getComponent("CSteeringMovement"));
		_combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));
	}

	//---------------------------------------------------------

	CStatsModifier::~CStatsModifier()
	{
		if(!modifications.empty())
		{
			for(std::map<std::string,StatModification*>::const_iterator it = modifications.begin();
				it != modifications.end(); ++it)
					delete it->second;
		}
	}

	//---------------------------------------------------------

	void CStatsModifier::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(modifications.size() > 0){
			for(std::map<std::string,StatModification*>::iterator it = modifications.begin();
				it != modifications.end(); ++it)
			{
				
				if(it->second->endingTime > 0)
				{
					it->second->acumTime += msecs;

					if(it->second->acumTime >= it->second->endingTime)
					{
						it->second->restore = true;
						(*this.*modificationHandlers[it->second->type])((it->second));

						delete (it->second);

						 it = modifications.erase(it);
					}
				}
				if(it == modifications.end())
					break;
			}
		}
	}

	//---------------------------------------------------------

	bool CStatsModifier::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("ModifyStat") == 0) || (message->type.compare("RestoreStat") == 0);
	}

	//---------------------------------------------------------

	void CStatsModifier::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("ModifyStat") == 0)
		{			
			auto m = std::static_pointer_cast<ModifyStat>(message);
			StatModification* mod;
			//Si una modificación con el mismo ID ya está en la tabla de modificaciones,
			//la guardamos y operamos la restauración de los valores que había modificado.
			
			if(modifications.find(m->modID) != modifications.end())
			{
				mod = modifications[m->modID];
				mod->restore = true;
				(*this.*modificationHandlers[m->modType])(mod);
			}
			else
			{
				//En otro caso, creamos una nueva y la guardamos en el diccionario de modificaciones
				mod = new StatModification();
				modifications[m->modID] = mod;
			}

			//Asignamos los nuevos valores de la modificación
			mod->endingTime = m->durationMsecs;
			mod->percentage = m->percentage;			
			mod->acumTime = 0;
			mod->restore = false;
			
			//Operamos la modificación según el tipo de modificación que sea
			(*this.*modificationHandlers[m->modType])(mod);
			
		}
		else if (message->type.compare("RestoreStat") == 0)
		{
			auto m = std::static_pointer_cast<RestoreStat>(message);			
			
			//Comprobamos que existe la modificación a restaurar
			if(modifications.find(m->modID) != modifications.end())
			{
				StatModification* stat = modifications[m->modID];

				stat->restore = true;
				(*this.*modificationHandlers[stat->type])(stat);
				//Tras restaurar, la borramos y la sacamos del diccionario.
				delete modifications[m->modID];
				modifications.erase(modifications.find(m->modID));
			}
		}
	}	

	//---------------------------------------------------------

	void CStatsModifier::modifyLife(StatModification* mod)
	{
		mod->type = ModificationType::lifeMod;

		
		if(!mod->restore)
		//Si la modificación no tiene que ser restaurada, entonces tiene que ser aplicada
		{
			//La primera vez guardamos el valor original de la estadística de la unidad
			if(_originalLife == 0)
				_originalLife = _lifeComponent->getMaxLife();

			//Calculamos la modificación según el porcentaje asignado en el mensaje
			float modMaxLife =  (_originalLife *  mod->percentage)/100;			

			//Añadimos ese porcentaje a la estadística máxima actual (por si hay otros modificadores sobre la misma estadística)
			_lifeComponent->setMaxLife( _lifeComponent->getMaxLife() + modMaxLife);			
		}
		else
		//Si tiene que ser aplicada
		{
			//Volvemos a calcular la modificación según el porcentaje asignado en el mensaje
			float modMaxLife =  (_originalLife *  mod->percentage)/100;

			//Añadimos su valor negativo, para que sea la operación contraria a la realizada en la aplicación de la modificación
			_lifeComponent->setMaxLife( _lifeComponent->getMaxLife() + (modMaxLife * -1));
		}
	}

	//---------------------------------------------------------

	void CStatsModifier::modifyAttack(StatModification* mod)
	{
		mod->type = ModificationType::attackMod;
		if(!mod->restore)
		{
			if(_originalAttackDamage == 0)
				_originalAttackDamage = _combatComponent->getAttackDamage();
			
			float modAttackDamage =  (_originalAttackDamage * mod->percentage)/100;
			_combatComponent->setAttackDamage(_combatComponent->getAttackDamage() + (int)modAttackDamage);
		}
		else
		{
			float modAttackDamage =  (_originalAttackDamage * mod->percentage)/100;
			_combatComponent->setAttackDamage(_combatComponent->getAttackDamage() + ((int)modAttackDamage * -1));
		}
	}

	//---------------------------------------------------------

	void CStatsModifier::modifySpeed(StatModification* mod)
	{
		mod->type = ModificationType::speedMod;
		if(!mod->restore)
		{	
			if(_originalMovementSpeed == 0)
				_originalMovementSpeed =  _steeringComponent->getMaxLinearSpeed();
			
			float modMaxLinearSpeed =  (_originalMovementSpeed * mod->percentage)/100;
			_steeringComponent->setMaxLinearSpeed(_steeringComponent->getMaxLinearSpeed() + modMaxLinearSpeed);
		}
		else
		{
			float modMaxLinearSpeed =  (_originalMovementSpeed * mod->percentage)/100;
			_steeringComponent->setMaxLinearSpeed(_steeringComponent->getMaxLinearSpeed() + (modMaxLinearSpeed * -1));	
		}
	}

	//---------------------------------------------------------

	void CStatsModifier::setBerserkerMode(StatModification* mod)
	{
		mod->type = ModificationType::berserker;
		_lifeComponent->setBerserkerMode(!mod->restore);
	}
}


