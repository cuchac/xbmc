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

#include <stdint.h>
#include <utility>
#include <vector>

namespace KODI
{
namespace DATASTORE
{
/*!
 * \brief Multicodec packed content type
 *
 * Matches the codes described in the spec:
 *
 * https://github.com/multiformats/multicodec/blob/master/table.csv
 */
enum class CIDCodec
{
  RAW = 0x55,
  DAG_FLATBUFFER = 0x72, //! @todo Upstream
};

/*!
 * \brief Implementation of the Content ID v1 spec for content-addressed
 *        identifiers
 */
class CCID
{
public:
  /*!
   * \brief Construct an empty CID
   */
  CCID() = default;

  /*!
   * \brief Construct a CID from parameters
   *
   * \param codec The CID codec
   * \param multihash The multihash
   */
  CCID(CIDCodec codec, std::vector<uint8_t> multihash);

  /*!
   * \brief Copy-construct a CID
   *
   * \param cid The CID being copied
   */
  CCID(const CCID& cid) = default;

  /*!
   * \brief Copy-assign a CID
   *
   * \param cid The CID being copied
   */
  CCID& operator=(const CCID& cid) = default;

  /*!
   * \brief Move-construct a CID
   *
   * Makes a copy of the object representation as if by std::memmove().
   *
   * \param cid The CID being moved
   */
  CCID(CCID&& cid) = default;

  /*!
   * \brief Move-assign a CID
   *
   * Makes a copy of the object representation as if by std::memmove().
   *
   * \param cid The CID being moved
   */
  CCID& operator=(CCID&& cid) = default;

  /*!
   * \brief Get the CID codec
   */
  CIDCodec Codec() const
  {
    return m_codec;
  }

  /*!
   * \brief Set the CID codec
   */
  void SetCodec(CIDCodec codec)
  {
    m_codec = codec;
  }

  /*!
   * \brief Get the multihash
   */
  const std::vector<uint8_t>& Multihash() const
  {
    return m_multihash;
  }

  /*!
   * \brief Set the multihash
   */
  void SetMultihash(std::vector<uint8_t> multihash)
  {
    m_multihash = std::move(multihash);
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
  // CID parameters
  CIDCodec m_codec = CIDCodec::RAW;
  std::vector<uint8_t> m_multihash;
};
} // namespace DATASTORE
} // namespace KODI
