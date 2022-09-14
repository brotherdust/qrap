# Find SpatiaLite
# ~~~
# Copyright (c) 2009, Sandro Furieri <a.furieri at lqt.it>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# CMake module to search for SpatiaLite library
#
# If it's found it sets SPATIALITE_FOUND to TRUE
# and following variables are set:
#    SPATIALITE_INCLUDE_DIR
#    SPATIALITE_LIBRARY

# This macro checks if the symbol exists
include(CheckLibraryExists)

# FIND_PATH and FIND_LIBRARY normally search standard locations before the
# specified paths. To search non-standard paths first, FIND_* is invoked first
# with specified paths and NO_DEFAULT_PATH and then again with no specified
# paths to search the default locations. When an earlier FIND_* succeeds,
# subsequent FIND_*s searching for the same item do nothing.

# try to use sqlite framework on mac want clean framework path, not unix
# compatibility path
if(APPLE)
  if(CMAKE_FIND_FRAMEWORK MATCHES "FIRST"
     OR CMAKE_FRAMEWORK_PATH MATCHES "ONLY"
     OR NOT CMAKE_FIND_FRAMEWORK)
    set(CMAKE_FIND_FRAMEWORK_save
        ${CMAKE_FIND_FRAMEWORK}
        CACHE STRING "" FORCE)
    set(CMAKE_FIND_FRAMEWORK
        "ONLY"
        CACHE STRING "" FORCE)
    find_path(SPATIALITE_INCLUDE_DIR SQLite3/spatialite.h)
    # if no spatialite header, we don't want sqlite find below to succeed
    if(SPATIALITE_INCLUDE_DIR)
      find_library(SPATIALITE_LIBRARY SQLite3)
      # FIND_PATH doesn't add "Headers" for a framework
      set(SPATIALITE_INCLUDE_DIR
          ${SPATIALITE_LIBRARY}/Headers
          CACHE PATH "Path to a file." FORCE)
    endif(SPATIALITE_INCLUDE_DIR)
    set(CMAKE_FIND_FRAMEWORK
        ${CMAKE_FIND_FRAMEWORK_save}
        CACHE STRING "" FORCE)
  endif()
endif(APPLE)

find_path(SPATIALITE_INCLUDE_DIR spatialite.h "$ENV{INCLUDE}"
          "$ENV{LIB_DIR}/include" "$ENV{LIB_DIR}/include/spatialite")

find_library(
  SPATIALITE_LIBRARY
  NAMES spatialite spatialite_i
  PATHS $ENV{LIB} $ENV{LIB_DIR}/lib)

if(SPATIALITE_INCLUDE_DIR AND SPATIALITE_LIBRARY)
  set(SPATIALITE_FOUND TRUE)
endif(SPATIALITE_INCLUDE_DIR AND SPATIALITE_LIBRARY)

if(SPATIALITE_FOUND)

  if(NOT SPATIALITE_FIND_QUIETLY)
    message(STATUS "Found SpatiaLite: ${SPATIALITE_LIBRARY}")
  endif(NOT SPATIALITE_FIND_QUIETLY)

  # Check for symbol gaiaDropTable
  if(APPLE)
    # no extra LDFLAGS used in link test, may fail in OS X SDK
    set(CMAKE_REQUIRED_LIBRARIES "-F/Library/Frameworks"
                                 ${CMAKE_REQUIRED_LIBRARIES})
  endif(APPLE)
  check_library_exists("${SPATIALITE_LIBRARY}" gaiaDropTable ""
                       SPATIALITE_VERSION_GE_4_0_0)
  check_library_exists("${SPATIALITE_LIBRARY}" gaiaStatisticsInvalidate ""
                       SPATIALITE_VERSION_G_4_1_1)
  check_library_exists("${SPATIALITE_LIBRARY}" spatialite_init_ex ""
                       SPATIALITE_HAS_INIT_EX)

else(SPATIALITE_FOUND)

  if(SPATIALITE_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find SpatiaLite")
  endif(SPATIALITE_FIND_REQUIRED)

endif(SPATIALITE_FOUND)
