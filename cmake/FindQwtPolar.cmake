# Find QwtPolar
# ~~~
# Copyright (c) 2011, Jürgen E. Fischer <jef at norbit.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Once run this will define:
#
# QWTPOLAR_FOUND       = system has QwtPolar lib
# QWTPOLAR_LIBRARY     = full path to the QwtPolar library
# QWTPOLAR_INCLUDE_DIR = where to find headers
#

find_path(
  QWTPOLAR_INCLUDE_DIR
  NAMES qwt_polar.h
  PATHS /usr/include /usr/local/include "$ENV{LIB_DIR}/include" "$ENV{INCLUDE}"
  PATH_SUFFIXES qwtpolar qwt)

find_library(
  QWTPOLAR_LIBRARY
  NAMES qwtpolar
  PATHS /usr/lib /usr/local/lib "$ENV{LIB_DIR}/lib" "$ENV{LIB}/lib")

if(QWTPOLAR_INCLUDE_DIR AND QWTPOLAR_LIBRARY)
  set(QWTPOLAR_FOUND TRUE)
endif(QWTPOLAR_INCLUDE_DIR AND QWTPOLAR_LIBRARY)

if(QWTPOLAR_FOUND)
  if(NOT QWTPOLAR_FIND_QUIETLY)
    message(STATUS "Found QwtPolar: ${QWTPOLAR_LIBRARY}")
  endif(NOT QWTPOLAR_FIND_QUIETLY)
else(QWTPOLAR_FOUND)
  if(QWTPOLAR_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find QwtPolar")
  endif(QWTPOLAR_FIND_REQUIRED)
endif(QWTPOLAR_FOUND)
