# CMake module to search for PostgreSQL library
#
# pg_config is searched for in POSTGRES_CONFIG dir, default /usr/bin
#
# If it's found it sets POSTGRES_FOUND to TRUE and following variables are set:
# POSTGRES_INCLUDE_DIR POSTGRES_LIBRARY

if(WIN32)
  if(NOT POSTGRES_INCLUDE_DIR)
    find_path(POSTGRES_INCLUDE_DIR libpq-fe.h /usr/local/include /usr/include
              c:/msys/local/include "$ENV{LIB_DIR}/include/postgresql")
  endif(NOT POSTGRES_INCLUDE_DIR)

  if(NOT POSTGRES_LIBRARY)
    find_library(
      POSTGRES_LIBRARY
      NAMES pq libpq
      PATHS /usr/local/lib /usr/lib c:/msys/local/lib "$ENV{LIB_DIR}/lib")
  endif(NOT POSTGRES_LIBRARY)

else(WIN32)
  if(UNIX)

    set(POSTGRES_CONFIG_PREFER_PATH
        "$ENV{POSTGRES_HOME}/bin"
        CACHE STRING "preferred path to PG (pg_config)")
    find_program(POSTGRES_CONFIG pg_config ${POSTGRES_CONFIG_PREFER_PATH}
                 /usr/bin/)
    # MESSAGE("DBG POSTGRES_CONFIG ${POSTGRES_CONFIG}")

    if(POSTGRES_CONFIG)
      # set INCLUDE_DIR
      exec_program(
        ${POSTGRES_CONFIG} ARGS
        --includedir
        OUTPUT_VARIABLE PG_TMP)
      set(POSTGRES_INCLUDE_DIR
          ${PG_TMP}
          CACHE STRING INTERNAL)

      # set LIBRARY_DIR
      exec_program(
        ${POSTGRES_CONFIG} ARGS
        --libdir
        OUTPUT_VARIABLE PG_TMP)
      if(APPLE)
        set(POSTGRES_LIBRARY
            ${PG_TMP}/libpq.dylib
            CACHE STRING INTERNAL)
      else(APPLE)
        set(POSTGRES_LIBRARY
            ${PG_TMP}/libpq.so
            CACHE STRING INTERNAL)
      endif(APPLE)
    endif(POSTGRES_CONFIG)

  endif(UNIX)
endif(WIN32)

if(POSTGRES_INCLUDE_DIR AND POSTGRES_LIBRARY)
  set(POSTGRES_FOUND TRUE)
endif(POSTGRES_INCLUDE_DIR AND POSTGRES_LIBRARY)

if(POSTGRES_FOUND)

  if(NOT POSTGRES_FIND_QUIETLY)
    message(STATUS "Found PostgreSQL: ${POSTGRES_LIBRARY}")
  endif(NOT POSTGRES_FIND_QUIETLY)

else(POSTGRES_FOUND)

  # SET (POSTGRES_INCLUDE_DIR "") SET (POSTGRES_LIBRARY "")

  if(POSTGRES_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find PostgreSQL")
  else(POSTGRES_FIND_REQUIRED)
    message(STATUS "Could not find PostgreSQL")
  endif(POSTGRES_FIND_REQUIRED)

endif(POSTGRES_FOUND)
