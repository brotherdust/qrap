# CMake module to search for GEOS library
#
# If it's found it sets GEOS_FOUND to TRUE and following variables are set:
# GEOS_INCLUDE_DIR GEOS_LIBRARY

find_path(
  GEOS_INCLUDE_DIR
  geos_c.h
  /usr/local/include
  /usr/include
  /usr/include/geos
  # MSVC
  "$ENV{LIB_DIR}/include"
  # mingw
  c:/msys/local/include)

find_library(
  GEOS_LIBRARY
  NAMES geos
  PATHS /usr/local/lib
        /usr/lib
        # MSVC
        "$ENV{LIB_DIR}/lib"
        # mingw
        c:/msys/local/lib)

if(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
  set(GEOS_FOUND TRUE)
endif(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)

message(STATUS "GEOS Library: ${GEOS_LIBRARY}")
message(STATUS "GEOS Include: ${GEOS_INCLUDE_DIR}")

if(GEOS_FOUND)

  if(NOT GEOS_FIND_QUIETLY)
    message(STATUS "Found GEOS: ${GEOS_LIBRARY}")
  endif(NOT GEOS_FIND_QUIETLY)

else(GEOS_FOUND)

  if(GEOS_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find GEOS")
  endif(GEOS_FIND_REQUIRED)

endif(GEOS_FOUND)
