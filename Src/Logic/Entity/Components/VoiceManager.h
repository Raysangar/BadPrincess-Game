/**
@file VoiceManager.h


@see Logic::CGraphics
@see Logic::IComponent

@author Iv�n Le�n
@date June, 2015
*/
#ifndef __Logic_VoiceManager_H
#define __Logic_VoiceManager_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

//declaraci�n de la clase
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CVoiceManager(){};

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
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posici�n.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
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
