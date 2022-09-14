# macro that checks for grass installation in specified directory

macro(CHECK_GRASS G_PREFIX)

  find_path(GRASS_INCLUDE_DIR grass/version.h ${G_PREFIX}/include)

  set(GRASS_LIB_NAMES
      gis
      vect
      dig2
      dbmiclient
      dbmibase
      shape
      dgl
      rtree
      datetime
      linkm
      form
      gproj)

  set(GRASS_LIBRARIES "")

  foreach(LIB ${GRASS_LIB_NAMES})
    set(LIB_PATH NOTFOUND)
    find_library(LIB_PATH grass_${LIB} PATHS ${G_PREFIX}/lib)

    if(LIB_PATH)
      if(NOT GRASS_LIBRARIES STREQUAL NOTFOUND)
        set(GRASS_LIBRARIES ${GRASS_LIBRARIES} ${LIB_PATH})
      endif(NOT GRASS_LIBRARIES STREQUAL NOTFOUND)
    else(LIB_PATH)
      set(GRASS_LIBRARIES NOTFOUND)
    endif(LIB_PATH)

  endforeach(LIB)

  # LIB_PATH is only temporary variable, so hide it (is it possible to delete a
  # variable?)
  mark_as_advanced(LIB_PATH)

  if(GRASS_INCLUDE_DIR AND GRASS_LIBRARIES)
    set(GRASS_FOUND TRUE)
    set(GRASS_PREFIX ${G_PREFIX})
  endif(GRASS_INCLUDE_DIR AND GRASS_LIBRARIES)

  mark_as_advanced(GRASS_INCLUDE_DIR GRASS_LIBRARIES)

endmacro(CHECK_GRASS)

# ##############################################################################
# search for grass installations

# list of paths which to search - user's choice as first
set(GRASS_PATHS ${GRASS_PREFIX} /usr/lib/grass c:/msys/local)

# mac-specific path
if(APPLE)
  set(GRASS_PATHS ${GRASS_PATHS} /Applications/GRASS.app/Contents/Resources)
endif(APPLE)

if(WITH_GRASS)

  foreach(G_PREFIX ${GRASS_PATHS})
    if(NOT GRASS_FOUND)
      check_grass(${G_PREFIX})
    endif(NOT GRASS_FOUND)
  endforeach(G_PREFIX)

endif(WITH_GRASS)

# ##############################################################################

if(GRASS_FOUND)
  file(READ ${GRASS_INCLUDE_DIR}/grass/version.h VERSIONFILE)
  string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[^ ]+" GRASS_VERSION ${VERSIONFILE})

  if(NOT GRASS_FIND_QUIETLY)
    message(STATUS "Found GRASS: ${GRASS_PREFIX} (${GRASS_VERSION})")
  endif(NOT GRASS_FIND_QUIETLY)

  # openpty is currently needed for GRASS shell
  include(CheckFunctionExists)
  if(APPLE)
    set(CMAKE_REQUIRED_INCLUDES util.h)
  else(APPLE)
    set(CMAKE_REQUIRED_INCLUDES pty.h)
    set(CMAKE_REQUIRED_LIBRARIES util)
  endif(APPLE)
  check_function_exists(openpty HAVE_OPENPTY)

  # add 'util' library to the dependencies
  if(HAVE_OPENPTY AND NOT APPLE)
    find_library(
      OPENPTY_LIBRARY
      NAMES util
      PATHS /usr/local/lib /usr/lib c:/msys/local/lib)
    set(GRASS_LIBRARIES ${GRASS_LIBRARIES} ${OPENPTY_LIBRARY})
  endif(HAVE_OPENPTY AND NOT APPLE)

else(GRASS_FOUND)

  if(WITH_GRASS)

    if(GRASS_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find GRASS")
    else(GRASS_FIND_REQUIRED)
      message(STATUS "Could not find GRASS")
    endif(GRASS_FIND_REQUIRED)

  endif(WITH_GRASS)

endif(GRASS_FOUND)
