# FindLibqrencode
# --------
# Find the libqrencode library and headers
#
# This will define the following variables:
#
# LIBQRENCODE_FOUND - system has libqrencode library and headers
# LIBQRENCODE_INCLUDE_DIRS - the libqrencode include directories
# LIBQRENCODE_LIBRARIES - the libqrencode library directories

if(ENABLE_INTERNAL_LIBQRENCODE)
  include(ExternalProject)
  file(STRINGS ${CMAKE_SOURCE_DIR}/tools/depends/target/libqrencode/Makefile VER REGEX "^[ ]*VERSION[ ]*=.+$")
  string(REGEX REPLACE "^[ ]*VERSION[ ]*=[ ]*" "" LIBQRENCODE_VER "${VER}")

  # Allow user to override the download URL with a local tarball
  # Needed for offline build envs
  if(LIBQRENCODE_URL)
    get_filename_component(LIBQRENCODE_URL "${LIBQRENCODE_URL}" ABSOLUTE)
  else()
    # TODO
    #set(LIBQRENCODE_URL http://mirrors.kodi.tv/build-deps/sources/libqrencode-${LIBQRENCODE_VER}.tar.gz)
    set(LIBQRENCODE_URL https://fukuchi.org/works/qrencode/qrencode-${LIBQRENCODE_VER}.tar.gz)
  endif()
  if(VERBOSE)
    message(STATUS "LIBQRENCODE_URL: ${LIBQRENCODE_URL}")
  endif()

  set(LIBQRENCODE_INCLUDE_DIR ${CMAKE_BINARY_DIR}/${CORE_BUILD_DIR}/include CACHE INTERNAL "libqrencode include dir")
  set(LIBQRENCODE_LIBRARY ${CMAKE_BINARY_DIR}/${CORE_BUILD_DIR}/lib/libqrencode.a CACHE INTERNAL "libqrencode library")

  externalproject_add(libqrencode
                      URL ${LIBQRENCODE_URL}
                      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/${CORE_BUILD_DIR}/download
                      PREFIX ${CORE_BUILD_DIR}/libqrencode
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/${CORE_BUILD_DIR}
                                 -DCMAKE_BUILD_TYPE=Release
                                 -DWITH_TOOLS=OFF
                                 "${EXTRA_ARGS}"
                      BUILD_BYPRODUCTS ${LIBQRENCODE_LIBRARY})
  set_target_properties(libqrencode PROPERTIES FOLDER "External Projects"
                                    INTERFACE_INCLUDE_DIRECTORIES ${LIBQRENCODE_INCLUDE_DIR})
else()
  find_path(LIBQRENCODE_LIBRARY NAMES libqrencode.a)
  find_path(LIBQRENCODE_INCLUDE_DIR NAMES libqrencode.h)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libqrencode
                                  REQUIRED_VARS LIBQRENCODE_LIBRARY LIBQRENCODE_INCLUDE_DIR
                                  VERSION_VAR LIBQRENCODE_VER)

if(LIBQRENCODE_FOUND)
  set(LIBQRENCODE_LIBRARIES ${LIBQRENCODE_LIBRARY})
  set(LIBQRENCODE_INCLUDE_DIRS ${LIBQRENCODE_INCLUDE_DIR})

  if(NOT TARGET libqrencode)
    add_library(libqrencode UNKNOWN IMPORTED)
    set_target_properties(libqrencode PROPERTIES
                               FOLDER "External Projects"
                               INTERFACE_INCLUDE_DIRECTORIES ${LIBQRENCODE_INCLUDE_DIR})
  endif()
endif()

mark_as_advanced(LIBQRENCODE_LIBRARY LIBQRENCODE_INCLUDE_DIR)
