/*
 *      Copyright (C) 2017 Team Kodi
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "GUIPlayerList.h"
#include "games/addons/GameClient.h"
#include "games/addons/input/GameClientInput.h"
#include "games/controllers/guicontrols/GUIGameController.h"
#include "games/controllers/Controller.h"
#include "games/players/listproviders/GUIPlayerProvider.h"
#include "games/players/windows/GUIPlayerWindowDefines.h"
#include "games/GameServices.h" //! @todo
#include "guilib/GUIListItemLayout.h"
#include "ServiceBroker.h" //! @todo

#include <array>
#include <tinyxml.h>

using namespace KODI;
using namespace GAME;

//const CScroller CGUIPlayerList::m_scroller; //! @todo

CGUIPlayerList::CGUIPlayerList(int parentID, int controlID,
                               float posX, float posY,
                               float width, float height,
                               ORIENTATION orientation,
                               const CScroller& scroller) :
  CGUIListContainer(parentID, controlID, posX, posY, width, height, orientation, scroller, true)
{
  InitializeBaseContainer();
}

CGUIPlayerList::CGUIPlayerList(const CGUIPlayerList& other) :
  CGUIListContainer(other)
{
  InitializeBaseContainer();
}

void CGUIPlayerList::InitializeBaseContainer()
{
  // Initialize CGUIBaseContainer
  m_listProvider = new CGUIPlayerProvider(GetParentID());
}

void CGUIPlayerList::UpdatePlayers()
{
  // Get controllers that currently belong to players
  if (m_gameClient)
  {
    auto playerProvider = static_cast<CGUIPlayerProvider*>(m_listProvider);

    ControllerVector players = m_gameClient->Input().GetPlayers();
    playerProvider->SetPlayers(players);
  }
}

void CGUIPlayerList::UpdateControllers()
{
  //! @todo Use correct controller, for now use default
  CGameServices& gameServices = CServiceBroker::GetGameServices();
  ControllerPtr defaultController = gameServices.GetDefaultController();

  std::array<CGUIListItemLayout*, 2> layouts{
    {
      m_layout,
      m_focusedLayout,
    }
  };

  for (auto layout : layouts)
  {
    if (layout == nullptr)
      continue;

    layout->WalkControls(
      [&defaultController](CGUIControl* control) -> void
      {
        CGUIGameController* controllerControl = dynamic_cast<CGUIGameController*>(control);
        if (controllerControl)
        {
          std::string controllerId;

          auto controller = controllerControl->GetController();
          if (controller)
            controllerId = controller->ID();

          if (controllerId != defaultController->ID())
            controllerControl->ActivateController(defaultController);
        }
      }
    );
  }
}

void CGUIPlayerList::OnSelect(unsigned int index)
{
  //! @todo
}
