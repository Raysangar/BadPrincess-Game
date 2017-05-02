/**
@file CStateMachine.h

En este fichero se define la clase CStateMachine,
que es la clase padre de las máquinas de estado,
CStateMachineFactory, factoría de máquinas de estado
para las prácticas, y CSMWander, una máquina de estado
de ejemplo.

@author Gonzalo Flórez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_StateMachine_H
#define __AI_StateMachine_H


#include "Condition.h"

#include "LatentAction.h"
#include "SimpleLatentActions.h"
#include "LARoutes.h"
#include "LAWorkmanInteractSO.h"
#include "LAExecuteSM.h"
#include "LAIdle.h"
#include "LAMoveTo.h"
#include "LASetTrap.h"
#include "LAChaseEnemy.h"
#include "LAAttackEnemy.h"
#include "LADie.h"
#include "LAFaint.h"
#include "LATaken.h"
#include "LABuild.h"
#include "LAWaitBuild.h"
#include "LAWaiting.h"
#include "LAThief\LAThiefInteractSO.h"
#include "LAKidnappingPrincess.h"
#include "LAGameOver.h"
#include "LAProtectKnight.h"
#include "LARequestSO.h"
#include "LAReachSO.h"
#include "LACarryEntity.h"
#include "LAChaseCarriedEntity.h"
#include "LAWaitCarriedEntity.h"
#include "LAThief\LAThiefDecision.h"
#include "LAStunned.h"
#include "LASpawn.h"
#include "LAIdleMenu.h"
#include "LAGatherMenu.h"

namespace Logic
{
	class CEntity;
}


namespace AI 
{

	/**
	Clase padre para las máquinas de estado.
	<p>
	Es una clase parametrizada. El parámetro es la clase de 
	los elementos en los nodos. En general, este parámetro será
	una acción ejecutable (CLatentAction).
	*/
	template <class TNode>
	class CStateMachine
	{
	public: 
		/**
		Constructor
		*/
		CStateMachine() : _entity(0), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Constructor que recibe la entidad que ejecuta la máquina de estado
		*/
		CStateMachine(Logic::CEntity* entity) : _entity(entity), _currentNodeId(-1), _initialNodeId(-1) { _edges = new EdgeList(); };
		/**
		Destructor
		*/
		~CStateMachine();
		/**
		Este método añade un nodo a la máquina de estado y devuelve un identificador
		del nodo. Este identificador se usa para referirse a los nodos al añadir
		aristas y establecer el nodo inicial.
		<p>
		Los nodos serán destruidos cuando se destruya la máquina de estado.

		@param content Contenido del nodo.
		@return Identificador para el nodo.
		*/
		int addNode(TNode *content);
		/**
		Añade una arista a la máquina de estado.
		<p>
		El método recibe los identificadores del nodo de origen y destino y una condición
		que indica cuándo se activa la transición de la arista.
		<p>
		Las condiciones serán destruidas cuando se destruya la máquina de estado.

		@param idOrig Identificador del nodo de origen.
		@param idDest Identificador del nodo de destino.
		@param cond Condición asociada a la arista.
		*/
		void addEdge(int idOrig, int idDest, ICondition<TNode> *cond);
		/**
		Este método comprueba las condiciones de las aristas que salen del 
		nodo actual y cambia de nodo si alguna de ellas es cierta. El método
		devuelve true si alguna transición es cierta (aunque no se cambie el
		nodo actual) y false en otro caso.
		<p>
		Las aristas se comprueban en el orden en que se han añadido y 
		la comprobación se detiene en el momento en que una de ellas se hace 
		cierta.
		*/
		bool nextState();
		/**
		Devuelve el contenido del nodo actual.
		*/
		TNode* getCurrentNode();
		/**
		Establece cuál es la entidad que ejecuta la máquina de estado.
		*/
		void setEntity(Logic::CEntity *entity) { _entity = entity; };
		/**
		Establece el nodo inicial.

		@param idNode Identificador del nodo inicial.
		*/
		void setInitialNode(int idNode) { _initialNodeId = idNode; };
		/**
		Reinicia la ejecución de la máquina de estado.
		*/
		void resetExecution(){ _currentNodeId = -1; };

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		/**
		Tipo que guarda la información de las aristas salientes de un nodo.
		Para cada arista tenemos un vector de pares con la condición y el 
		nodo de destino.
		*/
		typedef std::vector<std::pair<ICondition<TNode>*, int>> PairVector;
		/** 
		Tipo que guarda la información de todas las aristas. Está indexado 
		por el identificador del nodo de origen.
		*/
		typedef std::map<int, PairVector*> EdgeList;
		/**
		Entidad que ejecuta la máquina de estado.
		*/
		Logic::CEntity *_entity;
		/**
		Valores del nodo actual e inicial
		*/
		int _currentNodeId, _initialNodeId;
		/**
		Lista de nodos. Es un map que relaciona cada identificador de nodo con su contenido.
		*/
		std::map<int, TNode*> _nodes;
		/**
		Lista de aristas. Es un map que asocia cada nodo de origen de cada arista con una lista
		formada por pares (condición, nodo destino). Por ejemplo, si tenemos una aristas que sale
		del nodo con id 1 y va hasta el 2 y su condición es c1 y otra que va desde 1 hasta 3 y su 
		condición es c2, la estructura que tendremos será:
			{ 1 -> [ (c1, 2), 
					 (c2, 3) ] }
		*/
		EdgeList * _edges;

	}; // class CStateMachine
	
	/**
	Sub-state machine to represent the behaviour of build a well:
	*/
	class CSMWorkmanSetWell: public CStateMachine<CLatentAction>
	{
	public:
		CSMWorkmanSetWell(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeSetWell = this->addNode(new CLABuild(entity,false));
			int nodeWaitWell = this->addNode(new CLAWaitBuild(entity,false));

			this->addEdge(nodeMoveTo,nodeSetWell,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));//when the princess is far to the well
			
			this->addEdge(nodeSetWell,nodeWaitWell,new CConditionFail());//when the princess is far to the well

			this->addEdge(nodeWaitWell,nodeSetWell,new CConditionSuccess());//When the princess is near to the well
			
			this->setInitialNode(nodeMoveTo);
			this->resetExecution();
		}
	};
	/**
	Sub-state machine to represent the behaviour of build a well:
	*/
	class CSMWorkmanSetKitchen: public CStateMachine<CLatentAction>
	{
	public:
		CSMWorkmanSetKitchen(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeSetWell = this->addNode(new CLABuild(entity,false));
			int nodeWaitWell = this->addNode(new CLAWaitBuild(entity,false));

			this->addEdge(nodeMoveTo,nodeSetWell,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));//when the princess is far to the well
			
			this->addEdge(nodeSetWell,nodeWaitWell,new CConditionFail());//when the princess is far to the well

			this->addEdge(nodeWaitWell,nodeSetWell,new CConditionSuccess());//When the princess is near to the well
			
			this->setInitialNode(nodeMoveTo);
			this->resetExecution();
		}
	};
	/**
		Sub-state machine to represent the transport entity message:
	*/
	class CSMCarryEntity: public CStateMachine<CLatentAction>
	{
	public:
		CSMCarryEntity(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeCarryEntity = this->addNode(new CLACarryEntity(entity));

			this->addEdge(nodeMoveTo,nodeCarryEntity,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));//when the princess is far to the well

			this->addEdge(nodeCarryEntity,nodeCarryEntity,new CConditionSuccess());//When the princess is near to the well
			
			this->setInitialNode(nodeMoveTo);
			this->resetExecution();
		}
	};

	class CSMChaseCarriedEntity: public CStateMachine<CLatentAction>
	{
	public:
		CSMChaseCarriedEntity(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeChaseMoveTo = this->addNode(new CLAChaseCarriedEntity(entity));
			int nodeChaseIdle = this->addNode(new CLAWaitCarriedEntity(entity));

			this->addEdge(nodeChaseMoveTo,nodeChaseIdle,new CConditionSuccess());

			this->addEdge(nodeChaseIdle,nodeChaseMoveTo,new CConditionFail());
			
			this->setInitialNode(nodeChaseMoveTo);
			this->resetExecution();
		}
	};
	class CSMAttackEnemy: public CStateMachine<CLatentAction>
	{
	public:
		CSMAttackEnemy (Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeChase = this->addNode(new CLAChaseEnemy(entity));
			int nodeAttack = this->addNode(new CLAAttackEnemy(entity));
			this->addEdge(nodeChase,nodeAttack,new CConditionSuccess()); //When the entity reaches the enemy target
			this->addEdge(nodeAttack,nodeChase,new CConditionFail()); //When the enemy target is not at range anymore but still alive
			this->addEdge(nodeAttack,nodeChase,new CConditionMessage<CLatentAction>("AttackEnemy")); //When the player change enemy to attack
			this->setInitialNode(nodeChase);
			this->resetExecution();
		}
	};

	class CSMWorkmanInteract : public CStateMachine<CLatentAction>
	{
	public:
		CSMWorkmanInteract(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeReachTo = this->addNode(new CLAReachSO(entity));
			int nodeInteractSO = this->addNode(new CLAWorkmanInteractSO(entity));
			int nodeRequestSO = this->addNode(new CLARequestSO(entity));

			this->addEdge(nodeReachTo,nodeRequestSO,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeReachTo,nodeMoveTo,new CConditionFail());

			this->addEdge(nodeMoveTo,nodeRequestSO,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));

			this->addEdge(nodeRequestSO,nodeReachTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeRequestSO,nodeInteractSO,new CConditionMessage<CLatentAction>("SmartDataAvatarReady"));

			this->addEdge(nodeInteractSO,nodeReachTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));

			this->setInitialNode(nodeReachTo);
			this->resetExecution();
		}
	};

	class CSMWorkman : public CStateMachine<CLatentAction>
	{
	public:
		CSMWorkman(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeIdle = this->addNode(new CLAIdle(entity));
//			int nodeDecision = this->addNode(new CLAWorkmanDecision(entity));
			int nodeWorkmanInteract = this->addNode(new CLAExecuteSM(new CSMWorkmanInteract(entity)));
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeDie = this->addNode(new CLADie(entity));
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));
			int nodeSMBuildWell = this->addNode(new CLAExecuteSM(new CSMWorkmanSetWell(entity)));
			int nodeSMBuildKitchen = this->addNode(new CLAExecuteSM(new CSMWorkmanSetKitchen(entity)));
			int nodeSMCarryChest = this->addNode(new CLAExecuteSM(new CSMCarryEntity(entity)));
			int nodeSMChaseChest = this->addNode(new CLAExecuteSM(new CSMChaseCarriedEntity(entity)));

			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeIdle,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeIdle,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeIdle,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeIdle,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeIdle,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeIdle,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeIdle,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			
			//cambiare tutto questo e ripetere i nodi per tutti gli altri nodi
			//this->addEdge(nodeDecision,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			//this->addEdge(nodeDecision,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			//this->addEdge(nodeDecision,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			//this->addEdge(nodeDecision,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			//this->addEdge(nodeDecision,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			//this->addEdge(nodeDecision,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			//this->addEdge(nodeDecision,nodeDecision,new CConditionMessage<CLatentAction>("AvatarMoveToEntity"));//if we click over a entitywhen we are into this state (it should be impossible but it's a safety)
			//this->addEdge(nodeDecision,nodeMoveTo,new CConditionSuccess());//if we click over a entity who we don't react
			//this->addEdge(nodeDecision,nodeIdle,new CConditionFail());//if something go wrong


			this->addEdge(nodeWorkmanInteract,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeWorkmanInteract,nodeIdle,new CConditionFail());
			this->addEdge(nodeWorkmanInteract,nodeIdle,new CConditionMessage<CLatentAction>("AttemptSmartObjectLimitReached"));
			this->addEdge(nodeWorkmanInteract,nodeIdle,new CConditionMessage<CLatentAction>("SmartObjectFull"));
			this->addEdge(nodeWorkmanInteract,nodeIdle,new CConditionMessage<CLatentAction>("SmartObjectDisabled"));
			this->addEdge(nodeWorkmanInteract,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeWorkmanInteract,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeWorkmanInteract,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeWorkmanInteract,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeWorkmanInteract,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeWorkmanInteract,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeWorkmanInteract,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			

			this->addEdge(nodeMoveTo, nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeMoveTo,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeMoveTo,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeMoveTo,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeMoveTo,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeMoveTo,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeMoveTo,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeMoveTo,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));

			//this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeSMAttack,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMAttack,nodeIdle,new CConditionSuccess()); //When the enemy is destroyed 
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionFail()); //When the attack state is interrupted by an "AvatarRouteTo" message
			this->addEdge(nodeSMAttack,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeSMAttack,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMAttack,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeSMAttack,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeSMAttack,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeSMAttack,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));

			this->addEdge(nodeSMBuildWell,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMBuildWell,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeSMBuildWell,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMBuildWell,nodeIdle,new CConditionFail());
			this->addEdge(nodeSMBuildWell,nodeIdle,new CConditionSuccess());
			this->addEdge(nodeSMBuildWell,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeSMBuildWell,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMBuildWell,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeSMBuildWell,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeSMBuildWell,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeSMBuildWell,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));

			this->addEdge(nodeSMBuildKitchen,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMBuildKitchen,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeSMBuildKitchen,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMBuildKitchen,nodeIdle,new CConditionFail());
			this->addEdge(nodeSMBuildKitchen,nodeIdle,new CConditionSuccess());
			this->addEdge(nodeSMBuildKitchen,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeSMBuildKitchen,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMBuildKitchen,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeSMBuildKitchen,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeSMBuildKitchen,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeSMBuildKitchen,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));

			this->addEdge(nodeSMCarryChest,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMCarryChest,nodeIdle,new CConditionFail());
			this->addEdge(nodeSMCarryChest,nodeIdle,new CConditionMessage<CLatentAction>("InterruptTransport"));
			this->addEdge(nodeSMCarryChest,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeSMCarryChest,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMCarryChest,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeSMCarryChest,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeSMCarryChest,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeSMCarryChest,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			this->addEdge(nodeSMCarryChest,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));

			this->addEdge(nodeSMChaseChest,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMChaseChest,nodeIdle,new CConditionFail());
			this->addEdge(nodeSMChaseChest,nodeWorkmanInteract,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeSMChaseChest,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMChaseChest,nodeSMBuildWell,new CConditionMessage<CLatentAction>("BuildWell"));
			this->addEdge(nodeSMChaseChest,nodeSMBuildKitchen,new CConditionMessage<CLatentAction>("BuildKitchen"));
			this->addEdge(nodeSMChaseChest,nodeSMCarryChest,new CConditionMessage<CLatentAction>("TransportEntity"));
			this->addEdge(nodeSMChaseChest,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			this->addEdge(nodeSMChaseChest,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};

	

	/**
	Sub-state machine to represent the behaviour of setting a trap:
	Move to the trap position -> Set the trap	
	*/
	class CSMRangerSetTrap: public CStateMachine<CLatentAction>
	{
	public:
		CSMRangerSetTrap(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeSetTrap = this->addNode(new CLASetTrap(entity));
			this->addEdge(nodeMoveTo,nodeSetTrap,new CConditionMessage<CLatentAction>("AvatarFinishedRoute")); 
			this->addEdge(nodeSetTrap,nodeMoveTo,new CConditionFail());//When the entity receives another "SetTrap" message while it is setting a trap (never happens)
			this->setInitialNode(nodeMoveTo);
			this->resetExecution();
		}
	};

	/**
	Sub-state machine to represent the behaviour of setting a trap:
	Move to the trap position -> Set the trap	
	*/
	class CSMPrincessKidnapped: public CStateMachine<CLatentAction>
	{
	public:
		CSMPrincessKidnapped(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeFaint = this->addNode(new CLAFaint(entity));
			int nodeTaken = this->addNode(new CLATaken(entity));
			this->addEdge(nodeFaint,nodeTaken,new CConditionMessage<CLatentAction>("PrincessToTakenState"));
			this->addEdge(nodeTaken,nodeFaint,new CConditionMessage<CLatentAction>("PrincessLeft"));
			this->setInitialNode(nodeFaint);
			this->resetExecution();
		}
	};

	/**
	General state-machine to represent the global behaviour of the ranger,
	being Idle the root state.
	*/
	class CSMRangerGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMRangerGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{

			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeDie = this->addNode(new CLADie(entity));	
			int nodeSMTrap = this->addNode(new CLAExecuteSM(new CSMRangerSetTrap(entity)));
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));
			int nodeSpawn = this->addNode(new CLASpawn(entity));

			this->addEdge(nodeSpawn,nodeIdle,new CConditionSuccess());

			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeIdle,nodeSMTrap,new CConditionMessage<CLatentAction>("SetTrap"));
			this->addEdge(nodeIdle,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeIdle,nodeSMAttack,new CConditionMessage<CLatentAction>("DamageMessage"));
			this->addEdge(nodeIdle,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
						
			this->addEdge(nodeMoveTo,nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeMoveTo,nodeSMTrap,new CConditionMessage<CLatentAction>("SetTrap"));
			this->addEdge(nodeMoveTo,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeMoveTo,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			
			this->addEdge(nodeSMTrap,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMTrap,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeSMTrap,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeSMTrap,nodeIdle,new CConditionSuccess()); //When the trap has been set
			this->addEdge(nodeSMTrap,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeSMAttack,nodeIdle,new CConditionSuccess()); //When the enemy is destroyed 
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionFail()); //When the attack state is interrupted by an "AvatarRouteTo" message
			this->addEdge(nodeSMAttack,nodeSMTrap,new CConditionMessage<CLatentAction>("SetTrap"));
			this->addEdge(nodeSMAttack,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};

	/**
	General state-machine to represent the global behaviour of the warrior,
	being Idle the root state.
	*/
	class CSMWarriorGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMWarriorGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeDie = this->addNode(new CLADie(entity));	
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));

			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));			
			this->addEdge(nodeIdle,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeIdle,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
						
			this->addEdge(nodeMoveTo,nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));		
			this->addEdge(nodeMoveTo,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeMoveTo,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionMessage<CLatentAction>("ChangeEntityTypeIntoWell"));
			this->addEdge(nodeSMAttack,nodeIdle,new CConditionSuccess());//When the enemy is destroyed 
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionFail());//When the attack state is interrupted by an "AvatarRouteTo" message
			this->addEdge(nodeSMAttack,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};

	/**
	General state-machine to represent the global behaviour of the warrior,
	being Idle the root state.
	*/
	class CSMPrincessGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMPrincessGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));			
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));
			int nodeSMKidnapped = this->addNode(new CLAExecuteSM(new CSMPrincessKidnapped(entity)));

			this->addEdge(nodeIdle,nodeMoveTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));			
			this->addEdge(nodeIdle,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeIdle,nodeSMKidnapped,new CConditionMessage<CLatentAction>("EntityDying"));
						
			this->addEdge(nodeMoveTo,nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));		
			this->addEdge(nodeMoveTo,nodeSMAttack,new CConditionMessage<CLatentAction>("AttackEnemy"));
			this->addEdge(nodeMoveTo,nodeSMKidnapped,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeSMAttack,nodeIdle,new CConditionSuccess());//When the enemy is destroyed 
			this->addEdge(nodeSMAttack,nodeMoveTo,new CConditionFail());//When the attack state is interrupted by an "AvatarRouteTo" message
			this->addEdge(nodeSMAttack,nodeSMKidnapped,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeSMKidnapped,nodeIdle,new CConditionSuccess());//When the princess has awaken

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};

	class CSMThiefInteractChest: public CStateMachine<CLatentAction>
	{
	public:
		CSMThiefInteractChest(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodeReachTo = this->addNode(new CLAReachSO(entity));
			int nodeInteractSO = this->addNode(new CLAThiefInteractSO(entity));
			int nodeRequestSO = this->addNode(new CLARequestSO(entity));

			this->addEdge(nodeReachTo,nodeRequestSO,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeReachTo,nodeMoveTo,new CConditionFail());

			this->addEdge(nodeMoveTo,nodeRequestSO,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));

			this->addEdge(nodeRequestSO,nodeReachTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeRequestSO,nodeInteractSO,new CConditionMessage<CLatentAction>("SmartDataAvatarReady"));

			this->addEdge(nodeInteractSO,nodeReachTo,new CConditionMessage<CLatentAction>("AvatarRouteTo"));

			this->setInitialNode(nodeReachTo);
			this->resetExecution();
		}
	};
	/**
	General state-machine to represent the global behaviour of the Thief,
	being Idle the root state.
	*/
	class CSMThiefEnemyGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMThiefEnemyGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeDecision = this->addNode(new CLAThiefDecision(entity));
			int nodeWait = this->addNode(new CLAWaiting(entity, 3000)); // Wait for 3 seconds.
			int nodeInteractChest = this->addNode(new CLAExecuteSM(new CSMThiefInteractChest(entity)));
			int nodeEscape = this->addNode(new CLAMoveTo(entity));
			int nodeDie = this->addNode(new CLADie(entity));
			int nodeSMChaseChest = this->addNode(new CLAExecuteSM(new CSMChaseCarriedEntity(entity)));

			// TEST HAB. ATURDIR.
			//-------------------
			int nodeStunned = this->addNode(new CLAStunned(entity)); 
			this->addEdge(nodeStunned,nodeDecision,new CConditionSuccess());
			this->addEdge(nodeIdle,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			this->addEdge(nodeDecision,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			this->addEdge(nodeWait,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			this->addEdge(nodeInteractChest,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			this->addEdge(nodeEscape,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			this->addEdge(nodeSMChaseChest,nodeStunned,new CConditionMessage<CLatentAction>("StunHability"));
			//-------------------

			this->addEdge(nodeIdle,nodeDecision,new CConditionMessage<CLatentAction>("AvatarMoveToEntity"));
			this->addEdge(nodeIdle,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeEscape,nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeEscape,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));

			this->addEdge(nodeWait,nodeDecision,new CConditionMessage<CLatentAction>("AvatarMoveToEntity"));
			this->addEdge(nodeWait,nodeDecision,new CConditionSuccess());
			this->addEdge(nodeWait,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			
			this->addEdge(nodeDecision,nodeInteractChest,new CConditionMessage<CLatentAction>("InteractWithSmartObject"));
			this->addEdge(nodeDecision,nodeEscape,new CConditionMessage<CLatentAction>("ThiefEscape"));
			//this->addEdge(nodeDecision,nodeDisableTransport,new CConditionMessage<CLatentAction>("ThiefAttack")); //Disable chest movement.
			this->addEdge(nodeDecision,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeDecision,nodeSMChaseChest,new CConditionMessage<CLatentAction>("ChaseCarriedEntity"));
			this->addEdge(nodeDecision,nodeIdle,new CConditionFail());//if something go wrong
			
			this->addEdge(nodeInteractChest,nodeWait,new CConditionMessage<CLatentAction>("SmartObjectFull"));
			this->addEdge(nodeInteractChest,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeInteractChest,nodeIdle,new CConditionMessage<CLatentAction>("SmartObjectDisabled"));
			this->addEdge(nodeInteractChest,nodeIdle,new CConditionFail());
			this->addEdge(nodeInteractChest,nodeDecision,new CConditionMessage<CLatentAction>("AvatarMoveToEntity"));

			this->addEdge(nodeSMChaseChest,nodeDecision,new CConditionMessage<CLatentAction>("AvatarMoveToEntity"));
			this->addEdge(nodeSMChaseChest,nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMChaseChest,nodeIdle,new CConditionFail());

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};
	/**
	General state-machine to represent the global behaviour of the Knight,
	being Idle the root state.
	*/
	class CSMKnightGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMKnightGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeDie = this->addNode(new CLADie(entity));			
			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));
			int nodeKP = this->addNode(new CLAKidnappingPrincess(entity));
			int nodeMoveTo = this->addNode(new CLAMoveTo(entity));
			int nodePK = this->addNode(new CLAProtectKnight(entity));
			int nodeGO = this->addNode(new CLAGameOver(entity));
			int nodeStunned = this->addNode(new CLAStunned(entity));
			
			this->addEdge(nodeIdle, nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeIdle, nodeSMAttack, new CConditionMessage<CLatentAction>("KnightActivate"));
			this->addEdge(nodeIdle, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeSMAttack, nodeSMAttack, new CConditionSuccess());
			this->addEdge(nodeSMAttack, nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMAttack, nodeKP, new CConditionFail());
			this->addEdge(nodeSMAttack, nodePK, new CConditionMessage<CLatentAction>("ProtectKnight"));
			this->addEdge(nodeSMAttack, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeKP, nodeMoveTo, new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeKP, nodeDie, new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeKP, nodeSMAttack, new CConditionFail());
			this->addEdge(nodeKP, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodePK, nodeDie, new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodePK, nodeDie, new CConditionSuccess());
			this->addEdge(nodePK, nodeSMAttack, new CConditionMessage<CLatentAction>("KnightActivate"));
			this->addEdge(nodePK, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeMoveTo, nodeDie, new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeMoveTo, nodeGO, new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));
			this->addEdge(nodeMoveTo, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeStunned, nodeSMAttack, new CConditionSuccess());

			//this->addEdge(nodeGO, nodeDie, new CConditionSuccess()); [Ya no tienen porque morir ya que tenemos GameOver]
			this->addEdge(nodeGO, nodeMoveTo, new CConditionMessage<CLatentAction>("AvatarRouteTo"));

			this->setInitialNode(nodeSMAttack);
			this->resetExecution();
		}
	};

	/**
	General state-machine to represent the global behaviour of the Assassin,
	being Idle the root state.
	*/
	class CSMAssassinGeneral: public CStateMachine<CLatentAction>
	{
	public:
		CSMAssassinGeneral(Logic::CEntity* entity) : CStateMachine(entity) 
		{
			int nodeDie = this->addNode(new CLADie(entity));			
			int nodeIdle = this->addNode(new CLAIdle(entity));
			int nodeSMAttack = this->addNode(new CLAExecuteSM(new CSMAttackEnemy(entity)));
			int nodeStunned = this->addNode(new CLAStunned(entity));
			
			
			this->addEdge(nodeIdle, nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeIdle, nodeSMAttack, new CConditionMessage<CLatentAction>("NewTarget"));
			this->addEdge(nodeIdle, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeSMAttack, nodeDie,new CConditionMessage<CLatentAction>("EntityDying"));
			this->addEdge(nodeSMAttack, nodeSMAttack, new CConditionSuccess());
			this->addEdge(nodeSMAttack, nodeSMAttack, new CConditionMessage<CLatentAction>("NewTarget"));
			this->addEdge(nodeSMAttack, nodeIdle, new CConditionFail());
			this->addEdge(nodeSMAttack, nodeIdle, new CConditionMessage<CLatentAction>("MissionComplete"));
			this->addEdge(nodeSMAttack, nodeStunned, new CConditionMessage<CLatentAction>("StunHability"));

			this->addEdge(nodeStunned, nodeSMAttack, new CConditionSuccess());

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};


	/**
	General state-machine to represent the global behaviour of the Thief,
	being Idle the root state.
	*/
	class CSMMenuBehaviour: public CStateMachine<CLatentAction>
	{
	public:
		CSMMenuBehaviour(Logic::CEntity* entity) : CStateMachine(entity) 
		{			
			int nodeIdle = this->addNode(new CLAIdleMenu(entity));
			int nodeMove = this->addNode(new CLAMoveTo(entity));

			this->addEdge(nodeIdle,nodeMove,new CConditionMessage<CLatentAction>("AvatarRouteTo"));
			this->addEdge(nodeMove,nodeIdle,new CConditionMessage<CLatentAction>("AvatarFinishedRoute"));

			this->setInitialNode(nodeIdle);
			this->resetExecution();
		}
	};

	/**
	General state-machine to represent the global behaviour of the Thief,
	being Idle the root state.
	*/
	class CSMGatherMenuBehaviour: public CStateMachine<CLatentAction>
	{
	public:
		CSMGatherMenuBehaviour(Logic::CEntity* entity) : CStateMachine(entity) 
		{			
			int nodeGather = this->addNode(new CLAGatherMenu(entity));			

			this->setInitialNode(nodeGather);
			this->resetExecution();
		}
	};
	/**
	Factoría que devuelve máquinas de estado predefinidas.
	Sólo reconoce el valor "wander" como entrada, 
	que recorre puntos aleatorios del mapa
	*/
	class CStateMachineFactory 
	{
	public:
		static CStateMachine<CLatentAction>* getStateMachine(std::string smName, Logic::CEntity * entity)
		{
			if (smName == "workman") 
			{
				return new CSMWorkman(entity);
			}
			else if (smName == "ranger") 
			{
				return new CSMRangerGeneral(entity);
			}
			else if (smName == "warrior") 
			{
				return new CSMWarriorGeneral(entity);
			}
			else if (smName == "thief") 
			{
				return new CSMThiefEnemyGeneral(entity);
			}
			else if (smName == "knight")
			{
				return new CSMKnightGeneral(entity);
			}
			else if (smName == "menu") 
			{
				return new CSMMenuBehaviour(entity);
			}
			else if (smName == "gatherMenu") 
			{
				return new CSMGatherMenuBehaviour(entity);
			}
			else if (smName == "princess") 
			{
				return new CSMPrincessGeneral(entity);
			}
			else if (smName == "assassin")
				return new CSMAssassinGeneral(entity);

			return 0;
		}
	};

//////////////////////////////
//	Implementación de CStateMachine
//////////////////////////////
	template <class TNode>
	CStateMachine<TNode>::~CStateMachine() 
	{
		// Borramos las aristas
		for (EdgeList::iterator it = _edges->begin(); it != _edges->end(); it++)
		{
			PairVector* v = it->second;
			for (PairVector::iterator it2 = v->begin(); it2 != v->end(); it2++) {
				// Borra la condición
				delete it2->first;
			}
			// Borra el vector
			delete v;
		}
		delete _edges;

		// Borramos los nodos
		for (std::map<int, TNode*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
		{
			delete it->second;
		}
		
	}
//////////////////////////////
	template <class TNode>
	int CStateMachine<TNode>::addNode(TNode* content)
	{
		// El nuevo nodo (content) tenemos que añadirlo a la lista
		// de nodos (_nodes) en la última posición
		// Y tenemos que devolver el id, que es la posición en la 
		// que lo hemos añadido
		int index = _nodes.size();

		_nodes[index] = content;

		return index;
	} // addNode
//////////////////////////////
	template <class TNode>
	void CStateMachine<TNode>::addEdge(int idOrig, int idDest, ICondition<TNode> *cond)
	{
		// 1. Buscamos en la lista de aristas (_edges) las que salen de idOrig
		// (_edges es un map indexado por el origen de cada arista)
		EdgeList::iterator it = _edges->find(idOrig);
		PairVector* vector;
		// 2.1 Si no hay ninguna arista saliente para ese nodo tendremos
		// que crear un nuevo PairVector (que es la estructura donde se almacenan
		// las aristas salientes) y asignarla.
		if(it == _edges->end())
		{
			vector = new PairVector();
			(*_edges)[idOrig] = vector;
		}
		else
		{
			vector = (*it).second;
		}
		// 2.2 Si ya hay una lista la sacamos
		// 3. Añadimos la condición y el nodo de destino a la lista
		vector->push_back(std::pair<ICondition<TNode>*,int>(cond,idDest));	
	} // addEdge
//////////////////////////////
	template <class TNode>
	bool CStateMachine<TNode>::nextState()
	{
		// Si la máquina no está inicializada, el nodo actual (_currentNodeId)
		// será -1. En ese caso la inicializamos asignándole al current el nodo 
		// inicial e indicando que sí que ha habido cambio de nodo
		if(_currentNodeId == -1)
		{
			_currentNodeId = _initialNodeId;
			return true;
		}
		// En la lista de aristas (_edges) buscamos las que salen del nodo actual
		// (_edges está indexada por el nodo de origen de las aristas).
		// Si hay alguna arista de salida ==>
		// Recorremos la lista de aristas de salida y para cada una de ellas
		// Comprobamos la condición de la arista (método check)
		// Si se verifica actualizamos el nodo actual y devolvemos true
		// Si no hay aristas o no se cumple ninguna condición devolvemos false
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if(it != _edges->end())
		{
			PairVector* vector = (*it).second;
			PairVector::iterator it;
			for(it = vector->begin(); it != vector->end(); ++it)
			{
				TNode* node = _nodes[_currentNodeId];
				if((*it).first->check(node,_entity))
				{
					_currentNodeId = (*it).second;
					return true;
				}
			}
		}
		return false;


	} // nextState
//////////////////////////////
	template <class TNode>
	TNode* CStateMachine<TNode>::getCurrentNode()
	{
		return _nodes[_currentNodeId];
	} // getCurrentNode
//////////////////////////////
	template <class TNode>
	bool CStateMachine<TNode>::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// Si no hay un nodo actual no hay aristas interesadas
		if (_currentNodeId == -1) 
			return false;
		// Buscamos la lista de aristas que salen del nodo actual
		EdgeList::iterator it = _edges->find(_currentNodeId);
		if (it != _edges->end()) {
			PairVector* vector = (*it).second;
			// Para cada elemento del vector (arista que sale del nodo actual)
			for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
				// Llamamos al accept de la condición
				if (edgeIt->first->acceptN(message))
					return true;
			}
		}
		return false;
	}
//////////////////////////////
	template <class TNode>
	void CStateMachine<TNode>::processN(const std::shared_ptr<NMessage> &message){
		// Si no hay un nodo actual no hay aristas interesadas así que lo primero es comprobar si hay un nodo válido en _currentNodeId
		if (_currentNodeId != -1) { 
			// Buscamos la lista de aristas que salen del nodo actual
			EdgeList::iterator it = _edges->find(_currentNodeId);
			if (it != _edges->end()) {
				PairVector* vector = (*it).second;
				// Para cada elemento del vector (arista que sale del nodo actual)
				for (PairVector::iterator edgeIt = vector->begin(); edgeIt != vector->end(); edgeIt++){
					// Llamamos al process de la condición
					if (edgeIt->first->acceptN(message))
						edgeIt->first->processN(message);
				}
			}
		}
	}
//////////////////////////////

} // namespace AI 

#endif // __AI_StateMachine_H
