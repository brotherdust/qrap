# use bison for .yy files

# search for bison
macro(FIND_BISON)
  if(NOT BISON_EXECUTABLE)
    if(MSVC)
      find_program(BISON_EXECUTABLE "$ENV{LIB_DIR}/bin/bison.exe")
    else(MSVC)
      find_program(BISON_EXECUTABLE bison)
    endif(MSVC)
    if(NOT BISON_EXECUTABLE)

      message(FATAL_ERROR "Bison not found - aborting")

    else(NOT BISON_EXECUTABLE)

      exec_program(
        ${BISON_EXECUTABLE} ARGS
        --version
        OUTPUT_VARIABLE BISON_VERSION_STR)
      # get first line in case it's multiline
      string(REGEX REPLACE "([^\n]+).*" "\\1" FIRST_LINE "${BISON_VERSION_STR}")
      # get version information
      string(REGEX REPLACE ".* ([0-9]+)\\.([0-9]+)" "\\1" BISON_VERSION_MAJOR
                           "${FIRST_LINE}")
      string(REGEX REPLACE ".* ([0-9]+)\\.([0-9]+)" "\\2" BISON_VERSION_MINOR
                           "${FIRST_LINE}")
      if(BISON_VERSION_MAJOR LESS 2)
        message(
          FATAL_ERROR
            "Bison version is too old (${BISON_VERSION_MAJOR}.${BISON_VERSION_MINOR}). Use 2.0 or higher."
        )
      endif(BISON_VERSION_MAJOR LESS 2)

    endif(NOT BISON_EXECUTABLE)
  endif(NOT BISON_EXECUTABLE)

endmacro(FIND_BISON)

macro(ADD_BISON_FILES _sources)
  find_bison()

  foreach(_current_FILE ${ARGN})
    get_filename_component(_in ${_current_FILE} ABSOLUTE)
    get_filename_component(_basename ${_current_FILE} NAME_WE)

    set(_out ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp)

    # bison options: -t add debugging facilities -d produce additional header
    # file (used in parser.l) -v produce additional *.output file with parser
    # states

    add_custom_command(
      OUTPUT ${_out}
      COMMAND ${BISON_EXECUTABLE} ARGS -o${_out} -d -v -t ${_in}
      DEPENDS ${_in})

    set(${_sources} ${${_sources}} ${_out})
  endforeach(_current_FILE)
endmacro(ADD_BISON_FILES)
