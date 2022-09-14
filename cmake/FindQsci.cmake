# Find QScintilla2 PyQt module
# ~~~
#
# QScintilla2 website: http://www.riverbankcomputing.co.uk/software/qscintilla/
#
# Find the installed version of QScintilla2 module. FindQsci should be called
# after Python has been found.
#
# This file defines the following variables:
#
# QSCI_FOUND - system has QScintilla2 PyQt module
#
# QSCI_MOD_VERSION_STR - The version of Qsci module as a human readable string.
#
# Copyright (c) 2012, Larry Shaffer <larrys@dakotacarto.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(QSCI_MOD_VERSION_STR)
  # Already in cache, be silent
  set(QSCI_FOUND TRUE)
else(QSCI_MOD_VERSION_STR)

  if(SIP_BUILD_EXECUTABLE)
    # SIP >= 5.0 path

    file(GLOB _qsci_metadata
         "${Python_SITEARCH}/QScintilla*.dist-info/METADATA")
    if(_qsci_metadata)
      file(READ ${_qsci_metadata} _qsci_metadata_contents)
      string(REGEX REPLACE ".*\nVersion: ([^\n]+).*$" "\\1"
                           QSCI_MOD_VERSION_STR ${_qsci_metadata_contents})
    else(_qsci_metadata)
      execute_process(
        COMMAND
          ${Python_EXECUTABLE} -c
          "from PyQt5.Qsci import QSCINTILLA_VERSION_STR; print(QSCINTILLA_VERSION_STR)"
        OUTPUT_VARIABLE QSCI_MOD_VERSION_STR)
    endif(_qsci_metadata)

    if(QSCI_MOD_VERSION_STR)
      set(QSCI_SIP_DIR "${PYQT5_SIP_DIR}")
      set(QSCI_FOUND TRUE)
    endif(QSCI_MOD_VERSION_STR)

  else(SIP_BUILD_EXECUTABLE)
    # SIP 4.x path

    find_file(
      _find_qsci_py FindQsci.py
      PATHS ${CMAKE_MODULE_PATH}
      NO_CMAKE_FIND_ROOT_PATH)

    set(QSCI_VER 5)

    execute_process(COMMAND ${Python_EXECUTABLE} ${_find_qsci_py} ${QSCI_VER}
                    OUTPUT_VARIABLE qsci_ver)

    if(qsci_ver)
      string(REGEX REPLACE "^qsci_version_str:([^\n]+).*$" "\\1"
                           QSCI_MOD_VERSION_STR ${qsci_ver})
      find_path(
        QSCI_SIP_DIR
        NAMES Qsci/qscimod5.sip
        PATHS ${PYQT5_SIP_DIR} ${SIP_DEFAULT_SIP_DIR})
      set(QSCI_FOUND TRUE)
    endif(qsci_ver)

  endif(SIP_BUILD_EXECUTABLE)

  if(QSCI_FOUND)
    if(NOT QSCI_FIND_QUIETLY)
      message(STATUS "Found QScintilla2 PyQt module: ${QSCI_MOD_VERSION_STR}")
    endif(NOT QSCI_FIND_QUIETLY)
  else(QSCI_FOUND)
    if(QSCI_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find QScintilla2 PyQt module")
    endif(QSCI_FIND_REQUIRED)
  endif(QSCI_FOUND)

endif(QSCI_MOD_VERSION_STR)
