/*
*  Copyright (C) 2023 Team Kodi
*  This file is part of Kodi - https://kodi.tv
*
*  SPDX-License-Identifier: GPL-2.0-or-later
*  See LICENSES/README.md for more information.
*/

#include "GameAgentInput.h"

#include "GameAgentJoystick.h"
#include "games/GameServices.h"
#include "peripherals/Peripherals.h"

using namespace KODI;
using namespace GAME;

CGameAgentInput::CGameAgentInput(PERIPHERALS::CPeripherals& peripheralManager)
  : m_peripheralManager(peripheralManager)
{
}

CGameAgentInput::~CGameAgentInput() = default;

void CGameAgentInput::Initialize(GAME::CGameServices& gameServices)
{
  m_gameServices = &gameServices;
}

void CGameAgentInput::Deinitialize()
{
  m_gameServices = nullptr;
}

void CGameAgentInput::RegisterPeripheralObserver(Observer* obs)
{
  m_peripheralManager.RegisterObserver(obs);
}

void CGameAgentInput::UnregisterPeripheralObserver(Observer* obs)
{
  m_peripheralManager.UnregisterObserver(obs);
}

PERIPHERALS::PeripheralVector CGameAgentInput::GetPeripherals() const
{
  PERIPHERALS::PeripheralVector peripherals;

  m_peripheralManager.GetPeripheralsWithFeature(peripherals, PERIPHERALS::FEATURE_KEYBOARD);
  m_peripheralManager.GetPeripheralsWithFeature(peripherals, PERIPHERALS::FEATURE_MOUSE);
  m_peripheralManager.GetPeripheralsWithFeature(peripherals, PERIPHERALS::FEATURE_JOYSTICK);

  return peripherals;
}

void CGameAgentInput::PrunePeripherals()
{
  PERIPHERALS::PeripheralVector peripherals = GetPeripherals();
  for (auto it = m_joysticks.begin(); it != m_joysticks.end();)
  {
    // Dereference iterator
    const std::string& peripheralLocation = it->first;

    // Check if peripheral is in the list of connected peripherals
    auto it2 = std::find_if(peripherals.begin(), peripherals.end(),
                            [peripheralLocation](const PERIPHERALS::PeripheralPtr& peripheral) {
                              return peripheral->Location() == peripheralLocation;
                            });

    if (it2 == peripherals.end())
    {
      // Remove peripheral
      it = m_joysticks.erase(it);
    }
    else
    {
      // Check next peripheral
      ++it;
    }
  }
}

bool CGameAgentInput::OpenJoystick(const std::string& peripheralLocation,
                                   const std::string& controllerProfile,
                                   IGameAgentJoystickHandler& joystickHandler)
{
  // Return success if joystick is already open
  if (m_joysticks.find(peripheralLocation) != m_joysticks.end())
    return true;

  PERIPHERALS::PeripheralPtr peripheral =
      m_peripheralManager.GetPeripheralAtLocation(peripheralLocation);
  if (!peripheral)
    return false;

  if (m_gameServices == nullptr)
    return false;

  const GAME::ControllerPtr controller = m_gameServices->GetController(controllerProfile);
  if (!controller)
    return false;

  m_joysticks[peripheralLocation] = std::make_unique<CGameAgentJoystick>(
      std::move(peripheral), std::move(controller), joystickHandler);

  return true;
}

void CGameAgentInput::CloseJoystick(const std::string& peripheralLocation)
{
  auto it = m_joysticks.find(peripheralLocation);
  if (it != m_joysticks.end())
    m_joysticks.erase(it);
}
