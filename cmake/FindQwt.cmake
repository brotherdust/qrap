# Find Qwt
# ~~~
# Copyright (c) 2010, Tim Sutton <tim at linfiniti.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Once run this will define:
#
# QWT_FOUND       = system has QWT lib
# QWT_LIBRARY     = full path to the QWT library
# QWT_INCLUDE_DIR = where to find headers
#

find_path(
  QWT_INCLUDE_DIR
  NAMES qwt.h
  PATHS /usr/include /usr/local/include "$ENV{LIB_DIR}/include" "$ENV{INCLUDE}"
  PATH_SUFFIXES qwt-qt4 qwt qwt5 qwt6)

find_library(
  QWT_LIBRARY
  NAMES qwt qwt5 qwt6 qwt-qt4 qwt5-qt4 qwt6-qt4
  PATHS /usr/lib /usr/local/lib "$ENV{LIB_DIR}/lib" "$ENV{LIB}")

if(QWT_INCLUDE_DIR AND QWT_LIBRARY)
  set(QWT_FOUND TRUE)
endif(QWT_INCLUDE_DIR AND QWT_LIBRARY)

if(QWT_FOUND)
  file(READ ${QWT_INCLUDE_DIR}/qwt_global.h qwt_header)
  string(REGEX REPLACE "^.*QWT_VERSION_STR +\"([^\"]+)\".*$" "\\1"
                       QWT_VERSION_STR "${qwt_header}")
  if(NOT QWT_FIND_QUIETLY)
    message(STATUS "Found Qwt: ${QWT_LIBRARY} (${QWT_VERSION_STR})")
  endif(NOT QWT_FIND_QUIETLY)
else(QWT_FOUND)
  if(QWT_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Qwt")
  endif(QWT_FIND_REQUIRED)
endif(QWT_FOUND)
