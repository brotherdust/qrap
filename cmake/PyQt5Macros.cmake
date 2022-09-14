# Macros for PyQt
# ~~~
# Copyright (c) 2009, Juergen E. Fischer <jef at norbit dot de>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

set(PYUIC_PROG_NAME pyuic5)
set(PYUIC_PROG_NAMES pyuic5)
set(PYRCC_PROG_NAME pyrcc5)

if(NOT PYUIC_PROGRAM)
  find_program(
    PYUIC_PROGRAM
    NAMES ${PYUIC_PROG_NAMES}
    PATHS $ENV{LIB_DIR}/bin)
  if(NOT PYUIC_PROGRAM)
    message(FATAL_ERROR "pyuic5 not found - aborting")
  endif(NOT PYUIC_PROGRAM)
endif(NOT PYUIC_PROGRAM)

# Adapted from QT4_WRAP_UI
macro(PYQT_WRAP_UI outfiles)
  set(PYUIC_WRAPPER_OUTPUT_DIRECTORY "${PYTHON_OUTPUT_DIRECTORY}")
  set(PYUIC_WRAPPER_PYTHON_EXECUTABLE "${Python_EXECUTABLE}")
  if(CMAKE_HOST_WIN32)
    if(USING_NINJA OR USING_NMAKE)
      set(PYUIC_WRAPPER "${CMAKE_SOURCE_DIR}/scripts/pyuic_wrapper.bat")
      set(PYUIC_WRAPPER_PATH "${QGIS_OUTPUT_DIRECTORY}/bin")
    else(USING_NINJA OR USING_NMAKE)
      set(PYUIC_WRAPPER "${CMAKE_SOURCE_DIR}/scripts/pyuic_wrapper.bat")
      set(PYUIC_WRAPPER_PATH "${QGIS_OUTPUT_DIRECTORY}/bin/${CMAKE_BUILD_TYPE}")
    endif(USING_NINJA OR USING_NMAKE)
  elseif(MINGW)
    # Clear all variables to invoke PYUIC_PROGRAM directly
    set(PYUIC_WRAPPER_OUTPUT_DIRECTORY "")
    set(PYUIC_WRAPPER_PYTHON_EXECUTABLE "")
  else()
    # TODO osx
    set(PYUIC_WRAPPER "${CMAKE_SOURCE_DIR}/scripts/pyuic_wrapper.sh")
    set(PYUIC_WRAPPER_PATH "${QGIS_OUTPUT_DIRECTORY}/lib${LIB_SUFFIX}")
  endif()

  foreach(it ${ARGN})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.py)
    add_custom_command(
      OUTPUT ${outfile}
      COMMAND
        ${PYUIC_WRAPPER} "${PYUIC_PROGRAM}" "${PYUIC_WRAPPER_PATH}"
        "${PYUIC_WRAPPER_OUTPUT_DIRECTORY}" "${PYUIC_WRAPPER_PYTHON_EXECUTABLE}"
        ${infile} -o ${outfile}
      MAIN_DEPENDENCY ${infile}
      DEPENDS pygui pycore pyqtcompat)
    set(${outfiles} ${${outfiles}} ${outfile})
  endforeach(it)
endmacro(PYQT_WRAP_UI)

if(NOT PYRCC_PROGRAM)
  find_program(
    PYRCC_PROGRAM
    NAMES ${PYRCC_PROG_NAME}
    PATHS $ENV{LIB_DIR}/bin)
  if(NOT PYRCC_PROGRAM)
    message(FATAL_ERROR "pyrcc5 not found - aborting")
  endif(NOT PYRCC_PROGRAM)
endif(NOT PYRCC_PROGRAM)

# Adapted from QT4_ADD_RESOURCES
macro(PYQT_ADD_RESOURCES outfiles)
  foreach(it ${ARGN})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    get_filename_component(rc_path ${infile} PATH)
    set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}_rc.py)
    # parse file for dependencies all files are absolute paths or relative to
    # the location of the qrc file
    file(READ "${infile}" _RC_FILE_CONTENTS)
    string(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")
    set(_RC_DEPENDS)
    foreach(_RC_FILE ${_RC_FILES})
      string(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
      string(REGEX MATCH "^/|([A-Za-z]:/)" _ABS_PATH_INDICATOR "${_RC_FILE}")
      if(NOT _ABS_PATH_INDICATOR)
        set(_RC_FILE "${rc_path}/${_RC_FILE}")
      endif(NOT _ABS_PATH_INDICATOR)
      set(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
    endforeach(_RC_FILE)
    add_custom_command(
      OUTPUT ${outfile}
      COMMAND ${PYRCC_PROGRAM} ${_name_opt} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile}
      DEPENDS ${_RC_DEPENDS})
    set(${outfiles} ${${outfiles}} ${outfile})
  endforeach(it)
endmacro(PYQT_ADD_RESOURCES)
