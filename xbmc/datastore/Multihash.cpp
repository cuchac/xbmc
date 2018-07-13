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

#include "Multihash.h"

using namespace KODI;
using namespace DATASTORE;

CMultihash::CMultihash(MultihashEncoding encoding, std::vector<uint8_t> digest)
    : m_encoding(encoding)
    , m_digest(std::move(digest))
{
}

std::vector<uint8_t> CMultihash::Serialize() const
{
  std::vector<uint8_t> data;

  //! @todo

  return data;
}

void CMultihash::Deserialize(const std::vector<uint8_t>& data)
{
  //! @todo
}
