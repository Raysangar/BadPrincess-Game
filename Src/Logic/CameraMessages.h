#ifndef _Logic_CameraMessages_H
#define _Logic_CameraMessages_H

#include "Logic\NMessage.h"

DECLARE_MESSAGE_CLASS1P(CameraControl, Vector3, direction)
DECLARE_MESSAGE_CLASS1P(CameraPosition, Vector3, position)
DECLARE_MESSAGE_CLASS1P(CameraMoveTo, Vector3, position)
DECLARE_MESSAGE_CLASS1P(EnableCompositor, std::string, compositorName)
DECLARE_MESSAGE_CLASS1P(DisableCompositor, std::string, compositorName)
DECLARE_MESSAGE_CLASS1P(CameraScroll, float, scroll)

#endif