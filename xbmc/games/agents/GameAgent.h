/*
 *  Copyright (C) 2017-2022 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */
#pragma once

#include "peripherals/PeripheralTypes.h"

class CSetting;

namespace KODI
{
namespace GAME
{

class CGameAgent
{
public:
  CGameAgent(PERIPHERALS::PeripheralPtr peripheral);

  ~CGameAgent();

  PERIPHERALS::PeripheralPtr GetPeripheral() const { return m_peripheral; }

private:
  // Construction parameters
  const PERIPHERALS::PeripheralPtr m_peripheral;
};

} // namespace GAME
} // namespace KODI
