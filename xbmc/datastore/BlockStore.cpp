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

#include "BlockStore.h"
#include "Block.h"
#include "CID.h"
#include "DataStore.h"

using namespace KODI;
using namespace DATASTORE;

CBlockStore::CBlockStore(CDataStore& dataStore)
    : m_dataStore(dataStore)
{
}

bool CBlockStore::Has(const CCID& cid)
{
  std::vector<uint8_t> key = cid.Serialize();

  if (m_dataStore.Has(key.data(), key.size()))
    return true;

  return false;
}

bool CBlockStore::Get(const CCID& cid, CBlock& block)
{
  std::vector<uint8_t> key = cid.Serialize();

  if (m_dataStore.Get(key.data(), key.size(), block.Data()))
  {
    block.SetCID(cid);
    return true;
  }

  return false;
}

bool CBlockStore::Put(const CBlock& block)
{
  std::vector<uint8_t> key = block.CID().Serialize();

  if (m_dataStore.Put(key.data(), key.size(), block.Data().data(), block.Data().size()))
    return true;

  return false;
}

bool CBlockStore::Delete(const CCID& cid)
{
  std::vector<uint8_t> key = cid.Serialize();

  if (m_dataStore.Delete(key.data(), key.size()))
    return true;

  return false;
}
