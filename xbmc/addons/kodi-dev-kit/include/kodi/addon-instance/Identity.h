/*
 *  Copyright (C) 2014-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "../AddonBase.h"
#include "../c-api/addon-instance/identity.h"

#ifdef __cplusplus

namespace kodi
{
namespace addon
{

//==============================================================================
/// @addtogroup cpp_kodi_addon_identity
///
/// To use on Libretro and for stand-alone identitys or emulators that does not use
/// the Libretro API.
///

//==============================================================================
/// @addtogroup cpp_kodi_addon_identity
/// @brief @cpp_class{ kodi::addon::CInstanceIdentity }
/// **Identity add-on instance**\n
/// This class provides the basic identity processing system for use as an add-on in
/// Kodi.
///
/// This class is created at addon by Kodi.
///
class ATTRIBUTE_HIDDEN CInstanceIdentity : public IAddonInstance
{
public:
  //============================================================================
  /// @defgroup cpp_kodi_addon_identity_Base 1. Basic functions
  /// @ingroup cpp_kodi_addon_identity
  /// @brief **Functions to manage the addon and get basic information about it**
  ///
  ///@{

  //============================================================================
  /// @brief Identity class constructor
  ///
  /// Used by an add-on that only supports only Identity and only in one instance.
  ///
  /// This class is created at addon by Kodi.
  ///
  ///
  /// --------------------------------------------------------------------------
  ///
  ///
  /// **Here's example about the use of this:**
  /// ~~~~~~~~~~~~~{.cpp}
  /// #include <kodi/addon-instance/Identity.h>
  /// ...
  ///
  /// class ATTRIBUTE_HIDDEN CIdentityExample
  ///   : public kodi::addon::CAddonBase,
  ///     public kodi::addon::CInstanceIdentity
  /// {
  /// public:
  ///   CIdentityExample()
  ///   {
  ///   }
  ///
  ///   virtual ~CIdentityExample();
  ///   {
  ///   }
  ///
  ///   ...
  /// };
  ///
  /// ADDONCREATOR(CIdentityExample)
  /// ~~~~~~~~~~~~~
  ///
  CInstanceIdentity() : IAddonInstance(ADDON_INSTANCE_IDENTITY, GetKodiTypeVersion(ADDON_INSTANCE_IDENTITY))
  {
    if (CAddonBase::m_interface->globalSingleInstance != nullptr)
      throw std::logic_error("kodi::addon::CInstanceIdentity: Creation of more as one in single "
                             "instance way is not allowed!");

    SetAddonStruct(CAddonBase::m_interface->firstKodiInstance);
    CAddonBase::m_interface->globalSingleInstance = this;
  }
  //----------------------------------------------------------------------------

  //============================================================================
  /// @brief Destructor
  ///
  ~CInstanceIdentity() override = default;
  //----------------------------------------------------------------------------

  //============================================================================
  /// @brief **Callback to Kodi Function**\n
  /// The writable directory of the frontend.
  ///
  /// This directory can be used to store SRAM, memory cards, high scores,
  /// etc, if the identity client cannot use the regular memory interface,
  /// GetMemoryData().
  ///
  /// @return the used profile directory
  ///
  /// @remarks Only called from addon itself
  ///
  std::string ProfileDirectory() const { return m_instanceData->props->profile_directory; }
  //----------------------------------------------------------------------------

  //============================================================================
  ///
  /// @defgroup cpp_kodi_addon_identity_Operation 2. Identity operations
  /// @ingroup cpp_kodi_addon_identity
  /// @brief **Identity operations**
  ///
  /// These are mandatory functions for using this addon to get the available
  /// channels.
  ///
  ///
  ///---------------------------------------------------------------------------
  ///
  /// **Identity operation parts in interface:**\n
  /// Copy this to your project and extend with your parts or leave functions
  /// complete away where not used or supported.
  ///
  /// @copydetails cpp_kodi_addon_identity_Operation_header_addon_auto_check
  /// @copydetails cpp_kodi_addon_identity_Operation_source_addon_auto_check
  ///
  ///@{

  //============================================================================
  /// @brief Initialize the identity provider
  ///
  /// @param[in] profilePath The path to the user's writable profile
  /// @return the error, or @ref IDENTITY_ERROR_NO_ERROR if the identity was loaded
  ///
  virtual IDENTITY_ERROR Initialize(const std::string& url)
  {
    return IDENTITY_ERROR_NOT_IMPLEMENTED;
  }
  //----------------------------------------------------------------------------

  //============================================================================
  /// @brief Run the identity provider's event loop, if one is used
  ///
  /// @return the error, or @ref IDENTITY_ERROR_NO_ERROR if there was no error
  ///
  virtual IDENTITY_ERROR Run()
  {
    return IDENTITY_ERROR_NOT_IMPLEMENTED;
  }
  //----------------------------------------------------------------------------

  ///@}

private:
  void SetAddonStruct(KODI_HANDLE instance)
  {
    if (instance == nullptr)
      throw std::logic_error("kodi::addon::CInstanceIdentity: Creation with empty addon structure not"
                             "allowed, table must be given from Kodi!");

    m_instanceData = static_cast<AddonInstance_Identity*>(instance);
    m_instanceData->toAddon->addonInstance = this;

    m_instanceData->toAddon->Initialize = ADDON_Initialize;
    m_instanceData->toAddon->Run = ADDON_Run;
  }

  // --- Identity operations ---------------------------------------------------------

  inline static IDENTITY_ERROR ADDON_Initialize(const AddonInstance_Identity* instance, const char* profilePath)
  {
    return static_cast<CInstanceIdentity*>(instance->toAddon->addonInstance)->Initialize(profilePath);
  }

  inline static IDENTITY_ERROR ADDON_Run(const AddonInstance_Identity* instance)
  {
    return static_cast<CInstanceIdentity*>(instance->toAddon->addonInstance)->Run();
  }

  AddonInstance_Identity* m_instanceData;
};

} /* namespace addon */
} /* namespace kodi */

#endif /* __cplusplus */
