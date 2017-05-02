#include "StateMachineExecutor.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/WaypointGraph.h"
#include "AI/Server.h"

namespace Logic 
{

	IMP_FACTORY(CStateMachineExecutor);	

	//---------------------------------------------------------
	/**
	Inicialización del componente, utilizando la información extraída de
	la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
	behavior, que indica el nombre de la máquina de estado a ejecutar.

	@param entity Entidad a la que pertenece el componente.
	@param map Mapa Lógico en el que se registrará el objeto.
	@param entityInfo Información de construcción del objeto leído del
	fichero de disco.
	@return Cierto si la inicialización ha sido satisfactoria.
	*/
	bool CStateMachineExecutor::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		// Obtiene el nombre de la máquina de estado 
		if(entityInfo->hasAttribute("behavior")) {
			std::string smName = entityInfo->getStringAttribute("behavior");
			if (_currentStateMachine != 0) delete _currentStateMachine;
			_currentStateMachine = AI::CStateMachineFactory::getStateMachine(smName, _entity);
			// Saca una instancia de la máquina de estado de la factoría
			//_currentStateMachine = new AI::CSMPatrol(_entity);
		}

		return true;
	}

	 void CStateMachineExecutor::awake() {
		 _currentStateMachine->resetExecution();
		 if(_currentAction!=NULL)
			 _currentAction->reset();
		 _currentAction=NULL;
	 }
	//---------------------------------------------------------

	/**
	Método llamado en cada frame que actualiza el estado del componente.
	<p>
	Este método actualiza la máquina de estado. Si hay un cambio de estado, 
	se actualiza el valor del atributo _currentAction, que es el que contiene
	la acción latente que se está ejecutando. Por último, se llama al tick de
	la acción latente para que avance su ejecución.

	@param msecs Milisegundos transcurridos desde el último tick.
	*/
	void CStateMachineExecutor::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// En cada tick del ejecutor de máquinas de estado tenemos que 
		// realizar 2 acciones principales:
		// 1. Actualizar la máquina de estado.
		if(_currentStateMachine != NULL)
		{
			if(_currentStateMachine->nextState())
			{
				if(_currentAction != NULL)
					_currentAction->reset();
				_currentAction = _currentStateMachine->getCurrentNode();
			}
			// 1.1. Si la máquina cambia de estado reseteamos la acción que 
			//		estábamos ejecutando hasta ahora para que se pueda volver
			//		a ejecutar y cambiamos la acción latente actual por la del 
			//		nuevo estado.
			// 2. Ejecutar la acción latente correspondiente al estado actual
			_currentAction->tick(msecs);
		}
	}
	//---------------------------------------------------------
	/**
	Este método delega en el método accept de la acción latente que se 
	está ejecutando (_currentAction).
	*/
	bool CStateMachineExecutor::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// El método accept delega en el método accept de la acción latente actual		
		if (_currentStateMachine != NULL && _currentStateMachine->acceptN(message)) 
			return true; 
		if (_currentAction != NULL) 
			return _currentAction->acceptN(message);
		return false;
	}
	//---------------------------------------------------------
	/**
	Este método delega en el método process de la acción latente que se 
	está ejecutando (_currentAction).
	*/
	void CStateMachineExecutor::processN(const std::shared_ptr<NMessage> &message)
	{
		// El método process delega en el método process de la acción latente actual	
		if (_currentStateMachine != NULL)
			_currentStateMachine->processN(message);

		if(_currentStateMachine->nextState())
		{
			if(_currentAction != NULL)
				_currentAction->reset();

			_currentAction = _currentStateMachine->getCurrentNode();
		}

		if (_currentAction != NULL && _currentAction->acceptN(message)) 
			_currentAction->processN(message);
		
	}
	//---------------------------------------------------------

}