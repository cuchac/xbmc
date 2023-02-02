/*
 *  Copyright (C) 2017-2022 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GameAgent.h"

#include <cassert>

using namespace KODI;
using namespace GAME;

CGameAgent::CGameAgent(PERIPHERALS::PeripheralPtr peripheral) : m_peripheral(std::move(peripheral))
{
  //! @todo
  assert(m_peripheral.get() != nullptr);
}

CGameAgent::~CGameAgent() = default;
