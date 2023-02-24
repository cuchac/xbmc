/*
*  Copyright (C) 2023 Team Kodi
*  This file is part of Kodi - https://kodi.tv
*
*  SPDX-License-Identifier: GPL-2.0-or-later
*  See LICENSES/README.md for more information.
*/

#pragma once

namespace PERIPHERALS
{
class CPeripheral;
}

namespace KODI
{
namespace GAME
{
class IGameAgentJoystickHandler;
/*!
    * \ingroup games
    * \brief Handles game controller events for game agent functionality
    */
class IGameAgentJoystickHandler
{
public:
  virtual ~IGameAgentJoystickHandler() = default;

  //! @todo
};
} // namespace GAME
} // namespace KODI
