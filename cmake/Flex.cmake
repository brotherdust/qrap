# flex a .ll file

# search flex
macro(FIND_FLEX)
  if(NOT FLEX_EXECUTABLE)
    if(MSVC)
      find_program(FLEX_EXECUTABLE "$ENV{LIB_DIR}/bin/flex.exe")
    else(MSVC)
      find_program(FLEX_EXECUTABLE flex)
    endif(MSVC)
    if(NOT FLEX_EXECUTABLE)
      message(FATAL_ERROR "flex not found - aborting")
    endif(NOT FLEX_EXECUTABLE)
  endif(NOT FLEX_EXECUTABLE)
endmacro(FIND_FLEX)

macro(ADD_FLEX_FILES _sources)
  find_flex()

  foreach(_current_FILE ${ARGN})
    get_filename_component(_in ${_current_FILE} ABSOLUTE)
    get_filename_component(_basename ${_current_FILE} NAME_WE)

    set(_out ${CMAKE_CURRENT_BINARY_DIR}/flex_${_basename}.cpp)

    # -d option for flex means that it will produce output to stderr while
    # analyzing

    add_custom_command(
      OUTPUT ${_out}
      COMMAND ${FLEX_EXECUTABLE} ARGS -o${_out} -d ${_in}
      DEPENDS ${_in})

    set(${_sources} ${${_sources}} ${_out})
  endforeach(_current_FILE)
endmacro(ADD_FLEX_FILES)
