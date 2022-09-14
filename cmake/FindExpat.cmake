# CMake module to search for Expat library (library for parsing XML files)
#
# If it's found it sets EXPAT_FOUND to TRUE and following variables are set:
# EXPAT_INCLUDE_DIR EXPAT_LIBRARY

find_path(EXPAT_INCLUDE_DIR expat.h /usr/local/include /usr/include
          "$ENV{LIB_DIR}/include/expat" c:/msys/local/include)
# libexpat needed for msvc version
find_library(
  EXPAT_LIBRARY
  NAMES expat libexpat
  PATHS /usr/local/lib /usr/lib "$ENV{LIB_DIR}/lib" c:/msys/local/lib)

if(EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)
  set(EXPAT_FOUND TRUE)
endif(EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)

if(EXPAT_FOUND)

  if(NOT EXPAT_FIND_QUIETLY)
    message(STATUS "Found Expat: ${EXPAT_LIBRARY}")
  endif(NOT EXPAT_FIND_QUIETLY)

else(EXPAT_FOUND)

  if(EXPAT_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Expat")
  else(EXPAT_FIND_REQUIRED)
    if(NOT EXPAT_FIND_QUIETLY)
      message(STATUS "Could not find Expat")
    endif(NOT EXPAT_FIND_QUIETLY)
  endif(EXPAT_FIND_REQUIRED)

endif(EXPAT_FOUND)
