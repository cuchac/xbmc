/*
*  Copyright (C) 2023 Team Kodi
*  This file is part of Kodi - https://kodi.tv
*
*  SPDX-License-Identifier: GPL-2.0-or-later
*  See LICENSES/README.md for more information.
*/

#include "GameAgentJoystick.h"

#include "IGameAgentJoystickHandler.h"
#include "games/controllers/Controller.h"
#include "input/joysticks/interfaces/IInputProvider.h"
#include "peripherals/devices/Peripheral.h"

using namespace KODI;
using namespace GAME;

CGameAgentJoystick::CGameAgentJoystick(PERIPHERALS::PeripheralPtr peripheral,
                                       GAME::ControllerPtr controller,
                                       IGameAgentJoystickHandler& joystickHandler)
  : m_peripheral(std::move(peripheral)),
    m_controller(std::move(controller)),
    m_joystickHandler(joystickHandler)
{
  // Upcast peripheral to input interface
  JOYSTICK::IInputProvider* inputProvider = m_peripheral.get();

  // Register input handler to silently observe all input
  inputProvider->RegisterInputHandler(this, true);
}

CGameAgentJoystick::~CGameAgentJoystick()
{
  // Upcast peripheral to input interface
  JOYSTICK::IInputProvider* inputProvider = m_peripheral.get();

  // Unregister input handler
  inputProvider->UnregisterInputHandler(this);
}

std::string CGameAgentJoystick::ControllerID(void) const
{
  return m_controller->ID();
}

bool CGameAgentJoystick::HasFeature(const std::string& feature) const
{
  return true; // Capture input for all features
}

bool CGameAgentJoystick::AcceptsInput(const std::string& feature) const
{
  return true; // Accept input for all features
}

bool CGameAgentJoystick::OnButtonPress(const std::string& feature, bool bPressed)
{
  //! @todo
  return true;
}

bool CGameAgentJoystick::OnButtonMotion(const std::string& feature,
                                        float magnitude,
                                        unsigned int motionTimeMs)
{
  //! @todo
  return true;
}

bool CGameAgentJoystick::OnAnalogStickMotion(const std::string& feature,
                                             float x,
                                             float y,
                                             unsigned int motionTimeMs)
{
  //! @todo
  return true;
}

bool CGameAgentJoystick::OnAccelerometerMotion(const std::string& feature,
                                               float x,
                                               float y,
                                               float z)
{
  //! @todo
  return true;
}

bool CGameAgentJoystick::OnWheelMotion(const std::string& feature,
                                       float position,
                                       unsigned int motionTimeMs)
{
  //! @todo
  return true;
}

bool CGameAgentJoystick::OnThrottleMotion(const std::string& feature,
                                          float position,
                                          unsigned int motionTimeMs)
{
  //! @todo
  return true;
}

void CGameAgentJoystick::OnInputFrame()
{
  //! @todo
}
