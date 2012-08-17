
# CMake module to search for GEOS library
#
# If it's found it sets GEOS_FOUND to TRUE
# and following variables are set:
#    GEOS_INCLUDE_DIR
#    GEOS_LIBRARY


FIND_PATH(GEOS_INCLUDE_DIR geos_c.h 
  /usr/local/include 
  /usr/include
  /usr/include/geos 
  #MSVC
  "$ENV{LIB_DIR}/include"
  #mingw
  c:/msys/local/include
  )

FIND_LIBRARY(GEOS_LIBRARY NAMES geos PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
   SET(GEOS_FOUND TRUE)
ENDIF (GEOS_INCLUDE_DIR AND GEOS_LIBRARY)

MESSAGE(STATUS "GEOS Library: ${GEOS_LIBRARY}")
MESSAGE(STATUS "GEOS Include: ${GEOS_INCLUDE_DIR}")

IF (GEOS_FOUND)

   IF (NOT GEOS_FIND_QUIETLY)
      MESSAGE(STATUS "Found GEOS: ${GEOS_LIBRARY}")
   ENDIF (NOT GEOS_FIND_QUIETLY)

ELSE (GEOS_FOUND)

   IF (GEOS_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find GEOS")
   ENDIF (GEOS_FIND_REQUIRED)

ENDIF (GEOS_FOUND)
