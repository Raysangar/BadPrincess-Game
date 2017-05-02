/**
@file CommunicationPort.cpp

Contiene la implementación de la clase que se encarga del intercambio 
de los mensajes.

@see Logic::CCommunicationPort

@author David Llansó García
@date Julio, 2010
*/

#include "CommunicationPort.h"

namespace Logic {

	CCommunicationPort::~CCommunicationPort()
	{
		_nMessages.clear();

	} // ~CCommunicationPort
	
	//---------------------------------------------------------

	bool CCommunicationPort::setN(const std::shared_ptr<NMessage> message)
	{
		bool accepted = acceptN(message);
		if(accepted)
			_nMessages.push_back(message);

		return accepted;

	} // setN
	
	//---------------------------------------------------------

	void CCommunicationPort::processMessagesN()
	{
		NMessageList::const_iterator it = _nMessages.begin();
		for(; it != _nMessages.end(); it++)
			processN(*it);
		_nMessages.clear();

	} // processMessagesN

} // namespace Logic
