/*
 *  Copyright (C) 2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

class CAction;

namespace KODI
{
namespace ACTION
{
  /*!
   * \brief Handles events for Kodi actions
   */
  class IActionHandler
  {
  public:
    virtual ~IActionHandler() = default;

    /*!
     * \brief Handle Kodi actions
     *
     * \param action The action being handled
     *
     * \return True if the action was handled, false otherwise
     */
    virtual bool HandleAction(const CAction &action) = 0;
  };
}
}
