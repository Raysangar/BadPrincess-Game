/**
@file GUIManager.h

Contiene la implementaci�n del componente que gestiona
los atributos �nicos en el Goblin Montaraz

@see Logic::IComponent

@author Iv�n Le�n
@date Febrero, 2015
*/

#ifndef __Logic_GUIManager_H
#define __Logic_GUIManager_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

namespace Logic 
{
	class CEntity;	
	class CCarrierState;
	class CEntityTypeExchanger;

	class CGUIManager : public IComponent
	{

	DEC_FACTORY(CGUIManager);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CGUIManager(std::string componentName) : IComponent(componentName),_portraitName(""),_abilityQName(""),_abilityWName(""),_abilityEName(""),_abilityRName(""),_abilityQTooltip("")
		,_abilityWTooltip(""),_abilityETooltip(""),_abilityRTooltip(""),_abilityQluaField(""),_abilityWluaField(""),_abilityEluaField(""),_abilityRluaField(""){}

		/**
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CGUIManager()
		{
		}
		
		virtual bool activate();

		virtual void deactivate();

		virtual void awake();
		
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

		std::string getPortraitName() const { return _portraitName; }

		std::string getAbilityQName() const { return _abilityQName; }
		std::string getAbilityQTooltip() const { return _abilityQTooltip; }
		std::string getAbilityQLuaField() const { return _abilityQluaField; }

		std::string getAbilityWName() const { return _abilityWName; }
		std::string getAbilityWTooltip() const { return _abilityWTooltip; }
		std::string getAbilityWLuaField() const { return _abilityWluaField; }

		std::string getAbilityEName() const { return _abilityEName; }
		std::string getAbilityETooltip() const { return _abilityETooltip; }
		std::string getAbilityELuaField() const { return _abilityEluaField; }

		std::string getAbilityRName() const { return _abilityRName; }
		std::string getAbilityRTooltip() const { return _abilityRTooltip; }
		std::string getAbilityRLuaField() const { return _abilityRluaField; }

	protected:

		friend CCarrierState;
		friend CEntityTypeExchanger;
		void setAbilityQName(std::string name)  { _abilityQName = name; }
		void setAbilityQTooltip(std::string name)  { _abilityQTooltip = name; }
		void setAbilityQLuaField(std::string name)  { _abilityQluaField = name; }

		void setAbilityWName(std::string name)  { _abilityWName = name; }
		void setAbilityWTooltip(std::string name)  { _abilityWTooltip = name; }
		void setAbilityWLuaField(std::string name)  { _abilityWluaField = name; }

		void setAbilityEName(std::string name)  { _abilityEName = name; }
		void setAbilityETooltip(std::string name)  { _abilityETooltip = name; }
		void setAbilityELuaField(std::string name)  { _abilityEluaField = name; }

		void setAbilityRName(std::string name)  { _abilityRName = name; }
		void setAbilityRTooltip(std::string name)  { _abilityRTooltip = name; }
		void setAbilityRLuaField(std::string name)  { _abilityRluaField = name; }

		std::string _portraitName;

		std::string _abilityQName;
		std::string _abilityQTooltip;
		std::string _abilityQluaField;

		std::string _abilityWName;
		std::string _abilityWTooltip;
		std::string _abilityWluaField;

		std::string _abilityEName;
		std::string _abilityETooltip;
		std::string _abilityEluaField;

		std::string _abilityRName;
		std::string _abilityRTooltip;
		std::string _abilityRluaField;

	}; 

	REG_FACTORY(CGUIManager);
} 

#endif
