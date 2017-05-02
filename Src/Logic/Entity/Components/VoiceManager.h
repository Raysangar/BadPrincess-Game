/**
@file VoiceManager.h


@see Logic::CGraphics
@see Logic::IComponent

@author Iván León
@date June, 2015
*/
#ifndef __Logic_VoiceManager_H
#define __Logic_VoiceManager_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que recibe la textura y e frustum para dibujar l'area de efecto en suelo
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class CVoiceManager : public IComponent
	{
		DEC_FACTORY(CVoiceManager);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CVoiceManager(std::string componentName): IComponent(componentName), _msecsForNextOrderVoice(0), _msecsForNextAttackVoice(0), _msecsForNextSelectionVoice(0)
		{};

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CVoiceManager(){};

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
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posición.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

	protected:

		std::string _orderVoice;
		int _msecsForNextOrderVoice;

		std::string _attackVoice;
		int _msecsForNextAttackVoice;

		std::string _selectionVoice;
		int _msecsForNextSelectionVoice;

		std::string getRandomVoice(std::string rootVoice) const;

	}; // class CGraphics

	REG_FACTORY(CVoiceManager);

} // namespace Logic

#endif // __Logic_Graphics_H
