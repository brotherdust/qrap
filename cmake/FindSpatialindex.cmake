# Find Spatialindex
# ~~~
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Once run this will define:
#
# SPATIALINDEX_FOUND       = system has Spatialindex lib
# SPATIALINDEX_LIBRARY     = full path to the Spatialindex library
# SPATIALINDEX_INCLUDE_DIR = where to find headers
#

find_path(
  SPATIALINDEX_INCLUDE_DIR
  NAMES SpatialIndex.h
  PATHS /usr/include /usr/local/include "$ENV{LIB_DIR}/include" "$ENV{INCLUDE}"
        "$ENV{OSGEO4W_ROOT}/include"
  PATH_SUFFIXES spatialindex)

find_library(
  SPATIALINDEX_LIBRARY
  NAMES spatialindex_i spatialindex
  PATHS /usr/lib /usr/local/lib "$ENV{LIB_DIR}/lib" "$ENV{LIB}/lib"
        "$ENV{OSGEO4W_ROOT}/lib")

if(SPATIALINDEX_INCLUDE_DIR AND SPATIALINDEX_LIBRARY)
  set(SPATIALINDEX_FOUND TRUE)
endif(SPATIALINDEX_INCLUDE_DIR AND SPATIALINDEX_LIBRARY)

if(SPATIALINDEX_FOUND)
  if(NOT SPATIALINDEX_FIND_QUIETLY)
    message(STATUS "Found Spatialindex: ${SPATIALINDEX_LIBRARY}")
  endif(NOT SPATIALINDEX_FIND_QUIETLY)
else(SPATIALINDEX_FOUND)
  if(SPATIALINDEX_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Spatialindex")
  endif(SPATIALINDEX_FIND_REQUIRED)
endif(SPATIALINDEX_FOUND)
