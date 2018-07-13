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

#pragma once

#include "CID.h"

#include <stdint.h>
#include <utility>
#include <vector>

namespace KODI
{
namespace DATASTORE
{
/*!
 * \brief Container of raw data with a CID identifier
 */
class CBlock
{
public:
  /*!
   * \brief Construct an empty block
   */
  CBlock() = default;

  /*!
   * \brief Construct a block from parameters
   *
   * \param cid The block CID
   * \param data The block data
   */
  CBlock(CCID cid, std::vector<uint8_t> data);

  /*!
   * \brief Get the CID
   */
  const CCID& CID() const
  {
    return m_cid;
  }

  /*!
   * \brief Set the CID
   */
  void SetCID(CCID cid)
  {
    m_cid = std::move(cid);
  }

  /*!
   * \brief Get the data (const)
   */
  const std::vector<uint8_t>& Data() const
  {
    return m_data;
  }

  /*!
   * \brief Get the data (mutable)
   */
  std::vector<uint8_t>& Data()
  {
    return m_data;
  }

  /*!
   * \brief Set the data
   */
  void SetData(std::vector<uint8_t> data)
  {
    m_data = std::move(data);
  }

  /*!
   * \brief Serialize as an array of bytes
   */
  std::vector<uint8_t> Serialize() const;

  /*!
   * \brief Deserialize an array of bytes
   */
  void Deserialize(const std::vector<uint8_t>& data);

private:
  // Block parameters
  CCID m_cid;
  std::vector<uint8_t> m_data;
};
} // namespace DATASTORE
} // namespace KODI
