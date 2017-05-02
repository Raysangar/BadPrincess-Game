/**
@file EnemyController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CEnemyController
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/

#include "EnemyController.h"

#include "Logic/UnitMessages.h"
#include "Logic/Entity/Entity.h"


namespace Logic 
{
	IMP_FACTORY(CEnemyController);

	//--------------------------------------------------------

	CEnemyController::CEnemyController(std::string componentName) : IComponent(componentName)
		{			
		}
	
	//---------------------------------------------------------

	bool CEnemyController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CEnemyController::awake(){}

	//---------------------------------------------------------

	bool CEnemyController::activate()
	{			
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CEnemyController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CEnemyController::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarControl") == 0);
	} // accept
	//---------------------------------------------------------

	void CEnemyController::processN(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<AvatarControl> m = std::static_pointer_cast<AvatarControl>(message);
		if(!m->controlType.compare("turn"))
			turn(m->attribute);
		
			
	} // process

	void CEnemyController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}
	//---------------------------------------------------------

	void CEnemyController::turn(float amount) 
	{
		_entity->yawN(amount);

	} // turn
	

} // namespace Logic

