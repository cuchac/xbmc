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
 * \brief Hash algorithms used by multihash
 *
 * Matches the codes described in the spec:
 *
 * https://github.com/multiformats/multihash/blob/master/hashtable.csv
 */
enum class MultihashEncoding
{
  UNKNOWN = 0x00,
  MD5 = 0xd5,
  SHA1 = 0x11,
  SHA2_256 = 0x12,
  SHA2_512 = 0x13,
  SHA3_224 = 0x17,
  SHA3_256 = 0x16,
  SHA3_384 = 0x15,
  SHA3_512 = 0x14,
};

/*!
 * \brief Multihash is a protocol for combining size and encoding
 *        considerations
 */
class CMultihash
{
public:
  /*!
   * \brief Construct an empty multihash
   */
  CMultihash() = default;

  /*!
   * \brief Construct a multihash from parameters
   *
   * \param encoding The hash algorithm used to encode data
   * \param digest The hash digest
   */
  CMultihash(MultihashEncoding encoding, std::vector<uint8_t> digest);

  /*!
   * \brief Copy-construct a multihash
   *
   * \param multihash The multihash being copied
   */
  CMultihash(const CMultihash& multihash) = default;

  /*!
   * \brief Copy-assign a multihash
   *
   * \param multihash The multihash being copied
   */
  CMultihash& operator=(const CMultihash& multihash) = default;

  /*!
   * \brief Move-construct a multihash
   *
   * Makes a copy of the object representation as if by std::memmove().
   *
   * \param multihash The multihash being moved
   */
  CMultihash(CMultihash&& multihash) = default;

  /*!
   * \brief Move-assign a multihash
   *
   * Makes a copy of the object representation as if by std::memmove().
   *
   * \param multihash The multihash being moved
   */
  CMultihash& operator=(CMultihash&& multihash) = default;

  /*!
   * \brief Get the multihash encoding
   */
  MultihashEncoding Encoding() const
  {
    return m_encoding;
  }

  /*!
   * \brief Get the multihash digest
   */
  const std::vector<uint8_t>& Digest() const
  {
    return m_digest;
  }

  /*!
   * \brief Set the multihash properties
   */
  void SetProperties(MultihashEncoding encoding, std::vector<uint8_t> digest);

  /*!
   * \brief Serialize as an array of bytes
   */
  std::vector<uint8_t> Serialize() const;

  /*!
   * \brief Deserialize an array of bytes
   */
  void Deserialize(const std::vector<uint8_t>& data);

private:
  // Multihash parameters
  MultihashEncoding m_encoding = MultihashEncoding::UNKNOWN;
  std::vector<uint8_t> m_digest;
};
} // namespace DATASTORE
} // namespace KODI
