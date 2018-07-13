/*
 *      Copyright (C) 2018 Team Kodi
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

#include "CID.h"

using namespace KODI;
using namespace DATASTORE;

CCID::CCID(CIDCodec codec, std::vector<uint8_t> multihash)
    : m_codec(codec)
    , m_multihash(std::move(multihash))
{
}

std::vector<uint8_t> CCID::Serialize() const
{
  std::vector<uint8_t> data;

  //! @todo

  return data;
}

void CCID::Deserialize(const std::vector<uint8_t>& data)
{
  //! @todo
}
