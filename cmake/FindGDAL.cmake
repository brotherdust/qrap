# Once run this will define:
#
# GDAL_FOUND       = system has GDAL lib
#
# GDAL_LIBRARY     = full path to the library
#
# GDAL_INCLUDE_DIR      = where to find headers
#
# Magnus Homann

if(WIN32)

  if(MINGW)
    find_path(GDAL_INCLUDE_DIR gdal.h /usr/include /usr/include/gdal
              c:/msys/local/include)
    find_library(
      GDAL_LIBRARY
      NAMES gdal
      PATHS /usr/lib c:/msys/local/lib)
  endif(MINGW)

  if(MSVC)
    set(GDAL_INCLUDE_DIR
        "$ENV{LIB_DIR}/include/gdal"
        CACHE STRING INTERNAL)
    set(GDAL_LIBRARY
        "$ENV{LIB_DIR}/lib/gdal.lib"
        CACHE STRING INTERNAL)
  endif(MSVC)

else(WIN32)
  if(UNIX)

    # try to use framework on mac
    if(APPLE)
      set(GDAL_MAC_PATH /Library/Frameworks/GDAL.framework/unix/bin)
    endif(APPLE)

    set(GDAL_CONFIG_PREFER_PATH
        "$ENV{GDAL_HOME}/bin"
        CACHE STRING "preferred path to GDAL (gdal-config)")
    set(GDAL_CONFIG_PREFER_FWTOOLS_PATH
        "$ENV{FWTOOLS_HOME}/bin_safe"
        CACHE STRING "preferred path to GDAL (gdal-config) from FWTools")
    find_program(
      GDAL_CONFIG
      gdal-config
      ${GDAL_CONFIG_PREFER_PATH}
      ${GDAL_CONFIG_PREFER_FWTOOLS_PATH}
      ${GDAL_MAC_PATH}
      /usr/local/bin/
      /usr/bin/)
    # MESSAGE("DBG GDAL_CONFIG ${GDAL_CONFIG}")

    if(GDAL_CONFIG)
      # set INCLUDE_DIR to prefix+include
      exec_program(
        ${GDAL_CONFIG} ARGS
        --prefix
        OUTPUT_VARIABLE GDAL_PREFIX)
      # SET(GDAL_INCLUDE_DIR ${GDAL_PREFIX}/include CACHE STRING INTERNAL)
      find_path(GDAL_INCLUDE_DIR gdal.h ${GDAL_PREFIX}/include/gdal
                ${GDAL_PREFIX}/include /usr/include /usr/include/gdal)

      # extract link dirs for rpath
      exec_program(
        ${GDAL_CONFIG} ARGS
        --libs
        OUTPUT_VARIABLE GDAL_CONFIG_LIBS)

      # split off the link dirs (for rpath) use regular expression to match
      # wildcard equivalent "-L*<endchar>" with <endchar> is a space or a
      # semicolon
      string(REGEX MATCHALL "[-][L]([^ ;])+" GDAL_LINK_DIRECTORIES_WITH_PREFIX
                   "${GDAL_CONFIG_LIBS}")
      # MESSAGE("DBG
      # GDAL_LINK_DIRECTORIES_WITH_PREFIX=${GDAL_LINK_DIRECTORIES_WITH_PREFIX}")

      # remove prefix -L because we need the pure directory for LINK_DIRECTORIES

      if(GDAL_LINK_DIRECTORIES_WITH_PREFIX)
        string(REGEX REPLACE "[-][L]" "" GDAL_LINK_DIRECTORIES
                             ${GDAL_LINK_DIRECTORIES_WITH_PREFIX})
      endif(GDAL_LINK_DIRECTORIES_WITH_PREFIX)

      # split off the name use regular expression to match wildcard equivalent
      # "-l*<endchar>" with <endchar> is a space or a semicolon
      string(REGEX MATCHALL "[-][l]([^ ;])+" GDAL_LIB_NAME_WITH_PREFIX
                   "${GDAL_CONFIG_LIBS}")
      # MESSAGE("DBG  GDAL_LIB_NAME_WITH_PREFIX=${GDAL_LIB_NAME_WITH_PREFIX}")

      # remove prefix -l because we need the pure name

      if(GDAL_LIB_NAME_WITH_PREFIX)
        string(REGEX REPLACE "[-][l]" "" GDAL_LIB_NAME
                             ${GDAL_LIB_NAME_WITH_PREFIX})
      endif(GDAL_LIB_NAME_WITH_PREFIX)

      if(APPLE)
        set(GDAL_LIBRARY
            ${GDAL_LINK_DIRECTORIES}/lib${GDAL_LIB_NAME}.dylib
            CACHE STRING INTERNAL)
      else(APPLE)
        set(GDAL_LIBRARY
            ${GDAL_LINK_DIRECTORIES}/lib${GDAL_LIB_NAME}.so
            CACHE STRING INTERNAL)
      endif(APPLE)

    else(GDAL_CONFIG)
      message(
        "FindGDAL.cmake: gdal-config not found. Please set it manually. GDAL_CONFIG=${GDAL_CONFIG}"
      )
    endif(GDAL_CONFIG)

  endif(UNIX)
endif(WIN32)

if(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)
  set(GDAL_FOUND TRUE)
endif(GDAL_INCLUDE_DIR AND GDAL_LIBRARY)

if(GDAL_FOUND)

  if(NOT GDAL_FIND_QUIETLY)
    message(STATUS "Found GDAL: ${GDAL_LIBRARY}")
  endif(NOT GDAL_FIND_QUIETLY)

else(GDAL_FOUND)

  message(GDAL_INCLUDE_DIR=${GDAL_INCLUDE_DIR})
  message(GDAL_LIBRARY=${GDAL_LIBRARY})
  message(FATAL_ERROR "Could not find GDAL")

endif(GDAL_FOUND)
