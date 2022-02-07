/*
 *  Copyright (C) 2014-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#ifndef C_API_ADDONINSTANCE_IDENTITY_H
#define C_API_ADDONINSTANCE_IDENTITY_H

#include "../addon_base.h"

#include <stddef.h> /* size_t */

//==============================================================================
/// @ingroup cpp_kodi_addon_identity_Defs
/// @brief **Port ID used when topology is unknown**
#define DEFAULT_PORT_ID "1"
//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  //============================================================================
  /// @ingroup cpp_kodi_addon_identity_Defs
  /// @brief **Identity add-on error codes**
  ///
  /// Used as return values on most Identity related functions.
  ///
  typedef enum IDENTITY_ERROR
  {
    /// @brief no error occurred
    IDENTITY_ERROR_NO_ERROR,

    /// @brief an unknown error occurred
    IDENTITY_ERROR_UNKNOWN,

    /// @brief the method that the frontend called is not implemented
    IDENTITY_ERROR_NOT_IMPLEMENTED,

    /// @brief the command was rejected by the identity client
    IDENTITY_ERROR_REJECTED,

    /// @brief the parameters of the method that was called are invalid for this operation
    IDENTITY_ERROR_INVALID_PARAMETERS,

    /// @brief the command failed
    IDENTITY_ERROR_FAILED,

    /// @brief no identity is loaded
    IDENTITY_ERROR_NOT_LOADED,

    /// @brief identity requires restricted resources
    IDENTITY_ERROR_RESTRICTED,
  } IDENTITY_ERROR;
  //----------------------------------------------------------------------------

  /*!
   * @brief Identity properties
   *
   * Not to be used outside this header.
   */
  typedef struct AddonProps_Identity
  {
    /*!
     * The writable directory of the frontend
     */
    const char* profile_directory;
  } AddonProps_Identity;

  typedef void* KODI_IDENTITY_STREAM_HANDLE;

  /*! Structure to transfer the methods from kodi_identity_dll.h to Kodi */

  struct AddonInstance_Identity;

  /*!
   * @brief Identity callbacks
   *
   * Not to be used outside this header.
   */
  typedef struct AddonToKodiFuncTable_Identity
  {
    KODI_HANDLE kodiInstance;

    void (*CloseIdentity)(KODI_HANDLE kodiInstance);
  } AddonToKodiFuncTable_Identity;

  /*!
   * @brief Identity function hooks
   *
   * Not to be used outside this header.
   */
  typedef struct KodiToAddonFuncTable_Identity
  {
    KODI_HANDLE addonInstance;

    IDENTITY_ERROR(__cdecl* Initialize)(const struct AddonInstance_Identity*, const char*);
    IDENTITY_ERROR(__cdecl* Run)(const struct AddonInstance_Identity*);
  } KodiToAddonFuncTable_Identity;

  /*!
   * @brief Identity instance
   *
   * Not to be used outside this header.
   */
  typedef struct AddonInstance_Identity
  {
    struct AddonProps_Identity* props;
    struct AddonToKodiFuncTable_Identity* toKodi;
    struct KodiToAddonFuncTable_Identity* toAddon;
  } AddonInstance_Identity;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !C_API_ADDONINSTANCE_IDENTITY_H */
