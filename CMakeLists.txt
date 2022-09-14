project(qrap)

cmake_minimum_required(VERSION 3.22.1)
cmake_policy(SET CMP0005 NEW)

set(CMAKE_COLOR_MAKEFILE ON)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# in generated makefiles use relative paths so the project dir is moveable Note
# commented out since it cause problems but it would be nice to resolve these
# and enable
#
# issue is caused by INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}) near the end of
# this file generating incorrect path SET (CMAKE_USE_RELATIVE_PATHS ON)

set(WITH_DESKTOP
    TRUE
    CACHE BOOL "Determines whether QGIS desktop should be built")

# server disabled default because it needs FastCGI (which is optional
# dependency)
set(WITH_SERVER
    FALSE
    CACHE BOOL "Determines whether QGIS server should be built")
if(WITH_SERVER)
  set(SERVER_SKIP_ECW
      FALSE
      CACHE
        BOOL
        "Determines whether QGIS server should disable ECW (ECW in server apps requires a special license)"
  )

  set(WITH_SERVER_PLUGINS
      TRUE
      CACHE
        BOOL
        "Determines whether QGIS server support for python plugins should be built"
  )
  if(WITH_SERVER_PLUGINS)
    set(HAVE_SERVER_PYTHON_PLUGINS TRUE)
  endif(WITH_SERVER_PLUGINS)
endif(WITH_SERVER)

# Custom widgets
set(WITH_CUSTOM_WIDGETS
    FALSE
    CACHE
      BOOL
      "Determines whether QGIS custom widgets for Qt Designer should be built")

# build our version of astyle
set(WITH_ASTYLE
    FALSE
    CACHE
      BOOL
      "If you plan to contribute you should reindent with scripts/prepare-commit.sh (using 'our' astyle)"
)

# try to configure and build POSTGRESQL support
set(WITH_POSTGRESQL
    TRUE
    CACHE BOOL "Determines whether POSTGRESQL support should be built")
if(WITH_POSTGRESQL)
  set(POSTGRESQL_PREFIX
      ""
      CACHE PATH "Path to POSTGRESQL base directory")
endif(WITH_POSTGRESQL)

set(WITH_INTERNAL_QEXTSERIALPORT
    TRUE
    CACHE BOOL "Use internal build of Qextserialport")

set(WITH_QSPATIALITE
    FALSE
    CACHE BOOL "Determines whether QSPATIALITE sql driver should be built")

set(WITH_ORACLE
    FALSE
    CACHE BOOL "Determines whether Oracle support should be built")
if(WITH_ORACLE)
  set(HAVE_ORACLE TRUE)
  set(ORACLE_INCLUDEDIR
      ""
      CACHE STRING "Path to OCI headers")
  set(ORACLE_LIBDIR
      ""
      CACHE STRING "Path to OCI libraries")
endif(WITH_ORACLE)

# try to configure and build python bindings by default
set(WITH_BINDINGS
    TRUE
    CACHE BOOL "Determines whether python bindings should be built")
if(WITH_BINDINGS)
  # By default bindings will be installed only to QGIS directory Someone might
  # want to install it to python site-packages directory as otherwise user has
  # to use PYTHONPATH environemnt variable to add QGIS bindings to package
  # search path
  set(BINDINGS_GLOBAL_INSTALL
      FALSE
      CACHE BOOL
            "Install bindings to global python directory? (might need root)")
  set(WITH_STAGED_PLUGINS
      TRUE
      CACHE
        BOOL
        "Stage-install core Python plugins to run from build directory? (utilities and console are always staged)"
  )
  set(WITH_PY_COMPILE
      FALSE
      CACHE
        BOOL
        "Determines whether Python modules in staged or installed locations are byte-compiled"
  )
  # concatenate QScintilla2 API files
  set(WITH_QSCIAPI
      TRUE
      CACHE
        BOOL
        "Whether to generate PyQGIS QScintilla2 API file. (For devs) run 'make qsci-pap-src' in between QGIS build and install to regenerate .pap file in source tree for console auto-completion."
  )
  # keep casual users from updating their source tree via WITH_QSCIAPI
  mark_as_advanced(WITH_QSCIAPI)
  # path to custom Python framework on Mac
  if(APPLE)
    set(PYTHON_CUSTOM_FRAMEWORK
        ""
        CACHE
          PATH
          "Path to custom Python.framework on Mac. (should not have to specify other Python options)"
    )
  endif(APPLE)
endif(WITH_BINDINGS)

set(WITH_GLOBE
    FALSE
    CACHE BOOL "Determines whether Globe plugin should be built")
if(WITH_GLOBE)
  set(QT_USE_QTOPENGL 1)
  find_package(OSGEARTH REQUIRED)
  if(OSGEARTHQT_LIBRARY)
    # following variable is used in qgsconfig.h
    set(HAVE_OSGEARTHQT TRUE)
  endif(OSGEARTHQT_LIBRARY)
endif(WITH_GLOBE)

# Compile flag. Make it possible to turn it off.
set(PEDANTIC
    TRUE
    CACHE BOOL "Determines if we should compile in pedantic mode.")

# whether unit tests should be build
set(ENABLE_TESTS
    TRUE
    CACHE BOOL "Build unit tests?")

# whether coverage tests should be performed
set(ENABLE_COVERAGE
    FALSE
    CACHE BOOL "Perform coverage tests?")

# whether coverage documentation should be generated
set(GENERATE_COVERAGE_DOCS
    FALSE
    CACHE BOOL "Generate coverage docs (requires lcov)?")

# hide this variable because building of python bindings might fail if set to
# other directory than expected
mark_as_advanced(LIBRARY_OUTPUT_PATH)

if(MSVC AND CMAKE_GENERATOR MATCHES "NMake")
  # following variable is also used in qgsconfig.h
  set(USING_NMAKE TRUE)
endif(MSVC AND CMAKE_GENERATOR MATCHES "NMake")

# ##############################################################################
# check if lexer and parser are not missing
# http://www.mail-archive.com/cmake@cmake.org/msg02861.html

# INCLUDE(Flex)

# FIND_FLEX()

# IF (NOT FLEX_EXECUTABLE) MESSAGE(FATAL_ERROR "Couldn't find Flex") ENDIF (NOT
# FLEX_EXECUTABLE)

# INCLUDE(Bison)

# FIND_BISON()

# IF (NOT BISON_EXECUTABLE) MESSAGE(FATAL_ERROR "Couldn't find Bison") ENDIF
# (NOT BISON_EXECUTABLE)

# ##############################################################################
# search for dependencies

if(NOT WIN32 AND NOT ANDROID)
  include(CheckFunctionExists)
  check_function_exists(openpty OPENPTY_IN_LIBC)
  if(NOT OPENPTY_IN_LIBC)
    set(CMAKE_REQUIRED_INCLUDES util.h)
    set(CMAKE_REQUIRED_LIBRARIES util)
    check_function_exists(openpty NEED_LIBUTIL)
    if(NEED_LIBUTIL)
      set(OPENPTY_LIBRARY util)
    else(NEED_LIBUTIL)
      message(SEND_ERROR "openpty not found!")
    endif(NEED_LIBUTIL)
  endif(NOT OPENPTY_IN_LIBC)
endif(NOT WIN32 AND NOT ANDROID)

# required
find_package(Proj)
find_package(GEOS)
find_package(GDAL)
find_package(Expat REQUIRED)
find_package(Spatialindex REQUIRED)
find_package(Qwt REQUIRED)
message("  Found QWt Include: " ${QWT_INCLUDE_DIR})

find_package(Eigen REQUIRED)
# INCLUDE( ${EIGEN_INCLUDE_DIR} ) MESSAGE ( "  Found Eigen Include:"
# ${EIGEN_INCLUDE_DIR})

find_package(FANN REQUIRED)
include_directories(BEFORE ${FANN_INCLUDE_DIRS})

if(WITH_INTERNAL_QEXTSERIALPORT)
  set(QEXTSERIALPORT_INCLUDE_DIR
      ${CMAKE_SOURCE_DIR}/src/core/gps/qextserialport)
else(WITH_INTERNAL_QEXTSERIALPORT)
  find_package(Qextserialport REQUIRED)
endif(WITH_INTERNAL_QEXTSERIALPORT)

find_package(SQLite3)
if(NOT SQLite3_FOUND)
  message(SEND_ERROR "SQLite3 dependency was not found!")
endif(NOT SQLite3_FOUND)

# optional
if(WITH_POSTGRESQL)
  find_package(Postgres) # PostgreSQL provider, SPIT plugin
  find_package(PQXX)
endif(WITH_POSTGRESQL)

find_package(SPATIALITE REQUIRED)

if(SPATIALITE_VERSION_GE_4_0_0)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPATIALITE_VERSION_GE_4_0_0")
endif(SPATIALITE_VERSION_GE_4_0_0)
if(SPATIALITE_VERSION_G_4_1_1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPATIALITE_VERSION_G_4_1_1")
endif(SPATIALITE_VERSION_G_4_1_1)
if(SPATIALITE_HAS_INIT_EX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPATIALITE_HAS_INIT_EX")
endif(SPATIALITE_HAS_INIT_EX)

if(NOT PROJ_FOUND
   OR NOT GEOS_FOUND
   OR NOT GDAL_FOUND)
  message(SEND_ERROR "Some dependencies were not found!")
endif(
  NOT PROJ_FOUND
  OR NOT GEOS_FOUND
  OR NOT GDAL_FOUND)

if(POSTGRES_FOUND)
  # following variable is used in qgsconfig.h
  set(HAVE_POSTGRESQL TRUE)
endif(POSTGRES_FOUND)

# following variable is used in qgsconfig.h
set(HAVE_SPATIALITE TRUE)

# following variable is used in qgsconfig.h
set(HAVE_MSSQL TRUE)

# SET (WITH_QTWEBKIT TRUE CACHE INTERNAL "Enable QtWebkit support") IF
# (WITH_QTWEBKIT) ADD_DEFINITIONS(-DWITH_QTWEBKIT) ENDIF(WITH_QTWEBKIT)
# ##############################################################################
# search for Qt4
set(QT_MIN_VERSION 4.8.0)
set(ENABLE_QT5
    TRUE
    CACHE BOOL "If enabled will try to find Qt5 before looking for Qt4")
if(ENABLE_QT5)
  find_package(Qt5Core QUIET)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Xml REQUIRED)
  find_package(Qt5Svg REQUIRED)
  find_package(Qt5Concurrent REQUIRED)
  find_package(Qt5PrintSupport REQUIRED)
  find_package(Qt5Positioning)
  if(WITH_QTWEBKIT)
    find_package(Qt5WebKit REQUIRED)
    find_package(Qt5WebKitWidgets REQUIRED)
  endif(WITH_QTWEBKIT)
  find_package(Qt5Test REQUIRED)
  find_package(Qt5UiTools REQUIRED)
  find_package(Qt5Script REQUIRED)
  find_package(Qt5Sql REQUIRED)
  set(QT5_BUILD TRUE)
  include("cmake/modules/ECMQt4To5Porting.cmake")
  message(STATUS "Found Qt version: ${Qt5Core_VERSION_STRING}")
  # else() find_package(Qt4 ${QT_MIN_VERSION} REQUIRED) set(QT_USE_QTXML 1)
  # set(QT_USE_QTNETWORK 1) set(QT_USE_QTSVG 1) set(QT_USE_QTSQL 1)
  # set(QT_USE_QTWEBKIT 1) if(NOT QT_QTXML_FOUND OR NOT QT_QTNETWORK_FOUND OR
  # NOT QT_QTSVG_FOUND OR NOT QT_QTSQL_FOUND OR NOT QT_QTWEBKIT_FOUND OR
  # (WITH_CUSTOM_WIDGETS AND NOT QT_QTDESIGNER_FOUND)) message(SEND_ERROR "Some
  # Qt4 modules haven't been found!") endif( NOT QT_QTXML_FOUND OR NOT
  # QT_QTNETWORK_FOUND OR NOT QT_QTSVG_FOUND OR NOT QT_QTSQL_FOUND OR NOT
  # QT_QTWEBKIT_FOUND OR (WITH_CUSTOM_WIDGETS AND NOT QT_QTDESIGNER_FOUND))

  # include(${QT_USE_FILE})

  message(STATUS "Found Qt version: ${QTVERSION}")
endif()

# search for QScintilla2 (C++ lib)
find_package(QScintilla REQUIRED)

# ModelTest
set(ENABLE_MODELTEST
    FALSE
    CACHE BOOL "Enable QT ModelTest (not for production)")

if(SUPPRESS_QT_WARNINGS)
  # Newer versions of UseQt4.cmake include Qt with -isystem automatically This
  # can be used to force this behavior on older systems Can be removed as soon
  # as Travis-CI updates from precise
  include_directories(SYSTEM ${QT_INCLUDE_DIR})
endif(SUPPRESS_QT_WARNINGS)

# Disable automatic conversion from QString to ASCII 8-bit strings (char *)
# (Keeps code compatible with Qt/Mac/64bit)
add_definitions(-DQT_NO_CAST_TO_ASCII)

find_program(
  QT_LRELEASE_EXECUTABLE
  NAMES lrelease
  PATHS ${QT_BINARY_DIR}
  NO_DEFAULT_PATH)

# ##############################################################################
# enable use of c++11 features where available full c++11 support in clang 3.3+:
# http://clang.llvm.org/cxx_status.html for Mac, this is probably Apple LLVM 4.2
# (based on LLVM 3.2svn, in XCode 4.6+) or definitely Apple LLVM 5.0 (based on
# LLVM 3.3svn, in Xcode 5+): https://gist.github.com/yamaya/2924292

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  if(GCC_VERSION VERSION_GREATER 6.1 OR GCC_VERSION VERSION_EQUAL 6.1)
    set(USE_CXX_14 TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif(GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
    set(USE_CXX_11 TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  if((NOT APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "3.2")
     OR (APPLE AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.1"))
    set(USE_CXX_11 TRUE)
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-error=c++11-narrowing")
  endif()
elseif(MSVC AND MSVC_VERSION GREATER 1600)
  set(USE_CXX_11 TRUE)
else()
  set(USE_CXX_11 FALSE)
endif()

# allow override keyword if available
if(NOT USE_CXX_11)
  add_definitions("-Doverride=")
endif()

# ##############################################################################
# enable warnings

if(PEDANTIC)
  message(STATUS "Pedantic compiler settings enabled")
  if(MSVC)
    if(NOT USING_NMAKE)
      add_definitions(/W4)
    endif(NOT USING_NMAKE)

    # disable warnings
    add_definitions(/wd4100) # unused formal parameters
    add_definitions(/wd4127) # constant conditional expressions (used in Qt
                             # template classes)
    add_definitions(
      /wd4231) # nonstandard extension used : 'identifier' before template
               # explicit instantiation (used in Qt template classes)
    add_definitions(/wd4244) # conversion from '...' to '...' possible loss of
                             # data
    add_definitions(/wd4251) # needs to have dll-interface to be used by clients
                             # of class (occurs in Qt template classes)
    add_definitions(/wd4275) # non dll-interface class '...' used as base for
                             # dll-interface class '...'
    add_definitions(/wd4505) # unreferenced local function has been removed
                             # (QgsRasterDataProvider::extent)
    add_definitions(/wd4510) # default constructor could not be generated
                             # (sqlite3_index_info, QMap)
    add_definitions(/wd4512) # assignment operator could not be generated
                             # (sqlite3_index_info)
    add_definitions(/wd4610) # user defined constructor required
                             # (sqlite3_index_info)
    add_definitions(/wd4706) # assignment within conditional expression (pal)
    add_definitions(/wd4800) # 'int' : forcing value to bool 'true' or 'false'
                             # (performance warning)
  else(MSVC)
    # add warnings via flags (not as definitions as on Mac -Wall can not be
    # overridden per language )
    set(_warnings
        "-Wall -Wextra -Wno-long-long -Wformat-security -Wno-strict-aliasing")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_warnings}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_warnings}")

    # Qt produces lots of warnings with strict aliasing (as of Qt 4.4.0 & GCC
    # 4.3) There are redundant declarations in Qt and GDAL ADD_DEFINITIONS(
    # -fstrict-aliasing -Wstrict-aliasing=1 -Wredundant-decls )

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      set(CMAKE_CXX_FLAGS
          "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-overloaded-virtual"
      )
    endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

    # add any extra CXXFLAGS flags set by user. can be -D CXX_EXTRA_FLAGS or
    # environment variable command line -D option overrides environment variable
    # e.g. useful for suppressing transient upstream warnings in dependencies,
    # like Qt
    set(CXX_EXTRA_FLAGS
        ""
        CACHE STRING "Additional appended CXXFLAGS")
    if("${CXX_EXTRA_FLAGS}" STREQUAL "" AND DEFINED $ENV{CXX_EXTRA_FLAGS})
      set(CXX_EXTRA_FLAGS "$ENV{CXX_EXTRA_FLAGS}")
    endif("${CXX_EXTRA_FLAGS}" STREQUAL "" AND DEFINED $ENV{CXX_EXTRA_FLAGS})
    if(NOT "${CXX_EXTRA_FLAGS}" STREQUAL "")
      message(STATUS "Appending CXX_EXTRA_FLAGS")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_EXTRA_FLAGS}")
    endif(NOT "${CXX_EXTRA_FLAGS}" STREQUAL "")
  endif(MSVC)

endif(PEDANTIC)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Qunused-arguments")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
  set(CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS} -Qunused-arguments")
  set(CMAKE_MODULE_LINKER_FLAGS
      "${CMAKE_MODULE_LINKER_FLAGS} -Qunused-arguments")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Qunused-arguments")
endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)")
  # spatialite crashes on ppc - see bugs.debian.org/603986
  add_definitions(-fno-strict-aliasing)
endif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)")

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  message(STATUS "Debug output enabled")
  add_definitions(-DQGISDEBUG=1)
endif(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(MSVC)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
      "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DQGISDEBUG=1")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQGISDEBUG=1")
  # disable macros that offend std::numeric_limits<T>::min()/max()
  add_definitions(-DNOMINMAX)
endif(MSVC)

if(ENABLE_COVERAGE)
  include("cmake/modules/coverage/CodeCoverage.cmake")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  set(CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage"
  )
  set(CMAKE_MODULE_LINKER_FLAGS
      "${CMAKE_MODULE_LINKER_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage"
  )
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage"
  )
  setup_target_for_coverage(qgis_coverage ctest coverage)
endif(ENABLE_COVERAGE)

# ##############################################################################
# platform specific stuff

if(WIN32)
  set(DEFAULT_LIB_SUBDIR lib)
  set(DEFAULT_LIBEXEC_SUBDIR .)
  set(DEFAULT_DATA_SUBDIR .)
  set(DEFAULT_PLUGIN_SUBDIR plugins)
  set(DEFAULT_INCLUDE_SUBDIR include)

  if(MSVC)
    set(DEFAULT_BIN_SUBDIR bin)
    set(DEFAULT_CGIBIN_SUBDIR bin)
    # put all the build products into a single directory under build (doesnt
    # affect install target) to make for easier debugging.

    # Turn on defines for non standard maths stuff
    add_definitions(-D_USE_MATH_DEFINES)

    # Turn off deprecation warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)

    if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES
                                         RelWithDebInfo)
      if(NOT USING_NMAKE)
        message(STATUS "Generating browse files")
        add_definitions(/FR)
      endif(NOT USING_NMAKE)
    endif(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES
                                            RelWithDebInfo)

    if(INSTALL_DEPS)
      install(DIRECTORY ${INSTALL_DEPS} DESTINATION .)
    endif(INSTALL_DEPS)
  else(MSVC)
    set(DEFAULT_BIN_SUBDIR .)
    set(DEFAULT_CGIBIN_SUBDIR .)
  endif(MSVC)
else(WIN32)

  if(APPLE)
    if(POLICY CMP0042) # in CMake 3.0.0+
      set(CMAKE_MACOSX_RPATH OFF) # otherwise ON by default
    endif(POLICY CMP0042)
    # for Mac OS X, everything is put inside an application bundle save the root
    # install prefix for the app later
    set(QGIS_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(QGIS_MACAPP_PREFIX
        ${CMAKE_INSTALL_PREFIX}/${QGIS_APP_NAME}.app/Contents)
    # common prefix for components, let cmake handle it
    set(CMAKE_INSTALL_PREFIX ${QGIS_MACAPP_PREFIX}/MacOS)
    # 4 bundling levels, each includes previous 0  nothing 1  Qt frameworks 2
    # non-system libraries, "standard" 3  non-system frameworks, "standalone"
    set(QGIS_MACAPP_BUNDLE
        1
        CACHE STRING "What to bundle into app package")
    set(QGIS_MACAPP_BUNDLE_USER
        ""
        CACHE STRING "Path to user bundling script")
    set(QGIS_MACAPP_INSTALL_DEV
        FALSE
        CACHE BOOL "Install developer frameworks")
    set(QGIS_MACAPP_DEV_PREFIX
        "/Library/Frameworks"
        CACHE STRING "Path to install developer frameworks")
    # if we have @loader_path, >=OSX 10.5 (darwin 9+)
    if(CMAKE_SYSTEM_VERSION VERSION_GREATER 9.0.0 OR CMAKE_SYSTEM_VERSION
                                                     VERSION_EQUAL 9.0.0)
      set(OSX_HAVE_LOADERPATH 1)
    else()
      set(OSX_HAVE_LOADERPATH 0)
    endif()
    # this will define ${APP_SERVICES_LIBRARY}
    find_library(APP_SERVICES_LIBRARY ApplicationServices)

    set(DEFAULT_BIN_SUBDIR bin)
    set(QGIS_BIN_SUBDIR_REV ..)
    set(DEFAULT_CGIBIN_SUBDIR fcgi-bin)
    set(QGIS_CGIBIN_SUBDIR_REV ..)
    set(DEFAULT_LIB_SUBDIR lib)
    set(QGIS_LIB_SUBDIR_REV ..)
    set(QGIS_FW_SUBDIR ../Frameworks)
    set(QGIS_FW_SUBDIR_REV ../MacOS)
    set(DEFAULT_DATA_SUBDIR ../Resources)
    set(QGIS_DATA_SUBDIR_REV ../MacOS)
    set(DEFAULT_LIBEXEC_SUBDIR lib/qgis)
    set(QGIS_LIBEXEC_SUBDIR_REV ../..)
    set(DEFAULT_PLUGIN_SUBDIR ../PlugIns/qgis)
    set(QGIS_PLUGIN_SUBDIR_REV ../../MacOS)
    set(DEFAULT_INCLUDE_SUBDIR /usr/include/qgis)
    # path for framework references when running from build directory changed
    # later to reference in-app resources upon install
    set(CMAKE_INSTALL_NAME_DIR ${CMAKE_BINARY_DIR}/output/lib)
    if(WITH_GLOBE)
      set(OSG_PLUGINS_PATH
          ""
          CACHE PATH "Path to OSG plugins for bundling")
    endif(WITH_GLOBE)
    # recent cmakes force SDKs, recent SDKs don't have user symlinks need to
    # find non-system frameworks cmake bug #0007250 - CMAKE_SHARED_LINKER_FLAGS
    # ignored when creating a framework, so these need to be manually handled
    # with LINK_FLAGS options
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -F/Library/Frameworks")
    set(CMAKE_MODULE_LINKER_FLAGS
        "${CMAKE_MODULE_LINKER_FLAGS} -F/Library/Frameworks")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -F/Library/Frameworks")

  else(APPLE)
    # UNIX
    set(DEFAULT_BIN_SUBDIR bin)
    set(DEFAULT_CGIBIN_SUBDIR bin)
    set(DEFAULT_LIB_SUBDIR lib${LIB_SUFFIX})
    set(DEFAULT_DATA_SUBDIR share/qgis)
    set(DEFAULT_LIBEXEC_SUBDIR lib${LIB_SUFFIX}/qgis)
    set(DEFAULT_PLUGIN_SUBDIR lib${LIB_SUFFIX}/qgis/plugins)
    set(DEFAULT_INCLUDE_SUBDIR /usr/include/qgis)
  endif(APPLE)

endif(WIN32)

if(ANDROID)
  set(DEFAULT_PLUGIN_SUBDIR lib)
  set(DEFAULT_DATA_SUBDIR files/share)
endif(ANDROID)

# assume we have escaped compiler directives eventually we want to change this
# to new since we don't need to jump through so many hoops to escape compiler
# directives then
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

if(WIN32)
  set(DLLIMPORT "__declspec(dllimport)")
  set(DLLEXPORT "__declspec(dllexport)")
else(WIN32)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}"
                                                 MATCHES "Clang")
    set(DLLEXPORT "__attribute__ ((visibility (\\\"default\\\")))")
  else()
    set(DLLEXPORT "")
  endif()

  if(PEDANTIC AND NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    set(CMAKE_MODULE_LINKER_FLAGS
        "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined")
  endif(PEDANTIC AND NOT APPLE)

endif(WIN32)

add_definitions("-DCORE_EXPORT=${DLLIMPORT}")
add_definitions("-DGUI_EXPORT=${DLLIMPORT}")
add_definitions("-DPYTHON_EXPORT=${DLLIMPORT}")
add_definitions("-DANALYSIS_EXPORT=${DLLIMPORT}")
add_definitions("-DAPP_EXPORT=${DLLIMPORT}")
add_definitions("-DCUSTOMWIDGETS_EXPORT=${DLLIMPORT}")
add_definitions("-DSERVER_EXPORT=${DLLIMPORT}")

# ##############################################################################
# user-changeable settings which can be used to customize layout of QGIS
# installation (default values are platform-specific)

set(QGIS_BIN_SUBDIR
    ${DEFAULT_BIN_SUBDIR}
    CACHE STRING "Subdirectory where executables will be installed")
set(QGIS_CGIBIN_SUBDIR
    ${DEFAULT_CGIBIN_SUBDIR}
    CACHE STRING "Subdirectory where CGI executables will be installed")
set(QGIS_LIB_SUBDIR
    ${DEFAULT_LIB_SUBDIR}
    CACHE STRING "Subdirectory where libraries will be installed")
set(QGIS_LIBEXEC_SUBDIR
    ${DEFAULT_LIBEXEC_SUBDIR}
    CACHE STRING "Subdirectory where private executables will be installed")
set(QGIS_DATA_SUBDIR
    ${DEFAULT_DATA_SUBDIR}
    CACHE STRING "Subdirectory where QGIS data will be installed")
set(QGIS_PLUGIN_SUBDIR
    ${DEFAULT_PLUGIN_SUBDIR}
    CACHE STRING "Subdirectory where plugins will be installed")
set(QGIS_INCLUDE_SUBDIR
    ${DEFAULT_INCLUDE_SUBDIR}
    CACHE STRING "Subdirectory where header files will be installed")

# mark *_SUBDIR variables as advanced as this is not something that an average
# user would use
mark_as_advanced(
  QGIS_BIN_SUBDIR
  QGIS_CGIBIN_SUBDIR
  QGIS_LIB_SUBDIR
  QGIS_LIBEXEC_SUBDIR
  QGIS_DATA_SUBDIR
  QGIS_PLUGIN_SUBDIR
  QGIS_INCLUDE_SUBDIR)

# full paths for the installation
set(QGIS_BIN_DIR ${QGIS_BIN_SUBDIR})
set(QGIS_CGIBIN_DIR ${QGIS_CGIBIN_SUBDIR})
set(QGIS_LIB_DIR ${QGIS_LIB_SUBDIR})
set(QGIS_LIBEXEC_DIR ${QGIS_LIBEXEC_SUBDIR})
set(QGIS_DATA_DIR ${QGIS_DATA_SUBDIR})
set(QGIS_PLUGIN_DIR ${QGIS_PLUGIN_SUBDIR})
set(QGIS_INCLUDE_DIR ${QGIS_INCLUDE_SUBDIR})

# ##############################################################################
# QGIS and Q-Rap plugin paths
set(DEFAULT_BIN_SUBDIR /usr/bin)
set(DEFAULT_LIB_SUBDIR /usr/lib)
set(DEFAULT_DATA_SUBDIR /usr/share/qgis)
set(DEFAULT_PLUGIN_SUBDIR /usr/lib/qgis/plugins)
set(DEFAULT_INCLUDE_SUBDIR /usr/include/qgis)

# it's possible to set PLUGINS_ALSO_BINARIES to TRUE then some plugins that can
# run as standalone apps will be built also as standalone apps
set(PLUGINS_ALSO_BINARIES TRUE)

# ##############################################################################
# set the default locations where the targets (executables, libraries) will land
# when compiled this is to allow running qgis from the source tree without
# having to actually do a "make install"
set(QGIS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QGIS_OUTPUT_DIRECTORY}/${QGIS_BIN_SUBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QGIS_OUTPUT_DIRECTORY}/${QGIS_LIB_SUBDIR})

# write a marker with source directory path into the output's bin directory if
# run from the build directory QGIS will detect it and alter the paths
file(WRITE ${QGIS_OUTPUT_DIRECTORY}/${QGIS_BIN_SUBDIR}/qgisbuildpath.txt
     "${CMAKE_SOURCE_DIR}\n${QGIS_OUTPUT_DIRECTORY}")

# manual page - makes sense only on unix systems
if(UNIX AND NOT APPLE)
  set(DEFAULT_MANUAL_SUBDIR man)
  set(QGIS_MANUAL_SUBDIR
      ${DEFAULT_MANUAL_SUBDIR}
      CACHE STRING "Subdirectory where manual files will be installed")
  mark_as_advanced(QGIS_MANUAL_SUBDIR)
  set(QGIS_MANUAL_DIR ${CMAKE_INSTALL_PREFIX}/${QGIS_MANUAL_SUBDIR})
endif(UNIX AND NOT APPLE)

# ##############################################################################
# Python bindings

if(WITH_BINDINGS)

  # python support: check for interpreter, sip, pyqt5
  find_package(
    Python3
    COMPONENTS Interpreter
    REQUIRED)

  find_package(SIP REQUIRED)
  find_package(PyQt5 REQUIRED)
  find_package(Qsci REQUIRED)
  INCLUDE(PythonMacros) 
  INCLUDE(SIPMacros) 
  INCLUDE(PyQt5Macros)

  # setup SIP variables SEPARATE_ARGUMENTS(PYQT5_SIP_FLAGS) # convert space
  # separated values to a list SET(SIP_INCLUDES ${PYQT5_SIP_DIR}
  # ${CMAKE_SOURCE_DIR}/python) SET(SIP_CONCAT_PARTS 4) SET(SIP_EXTRA_OPTIONS
  # ${PYQT5_SIP_FLAGS})

  if(NOT BINDINGS_GLOBAL_INSTALL)
    set(PYTHON_SITE_PACKAGES_DIR ${QGIS_DATA_DIR}/python)
  endif(NOT BINDINGS_GLOBAL_INSTALL)

  if(WITH_CUSTOM_WIDGETS)
    set(PYUIC_WIDGET_PLUGIN_DIRECTORY ${PYQT5_MOD_DIR}/uic/widget-plugins/)
  endif(WITH_CUSTOM_WIDGETS)

endif(WITH_BINDINGS)

# ##############################################################################
# create qgsconfig.h installed with app target

configure_file(${CMAKE_SOURCE_DIR}/cmake_templates/qgsconfig.h.in
               ${CMAKE_BINARY_DIR}/qgsconfig.h)
include_directories(${CMAKE_BINARY_DIR})

# Added by Jef to prevent python core and gui libs linking to other qgisCore and
# qgisGui libs that may be in the same install prefix
link_directories(${CMAKE_BINARY_DIR}/src/core ${CMAKE_BINARY_DIR}/src/gui)

# ##############################################################################
# create qgsversion.h
find_file(GIT_MARKER index PATHS ${CMAKE_SOURCE_DIR}/.git)
if(GIT_MARKER)
  find_program(GITCOMMAND git PATHS c:/cygwin/bin)
  if(GITCOMMAND)
    if(MSVC)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
        COMMAND
          for /f \"usebackq tokens=1\" %%a in
          "(`\"${GITCOMMAND}\" log -n1 --oneline`)" do echo \#define QGSVERSION
          \"%%a\" >${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
        MAIN_DEPENDENCY ${GIT_MARKER}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    else(MSVC)
      execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GITCOMMAND} log -n1 --pretty=%h
        OUTPUT_VARIABLE REVISION)
      string(STRIP "${REVISION}" REVISION)
      # Get GIT remote and branch
      execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GITCOMMAND} name-rev --name-only HEAD
        OUTPUT_VARIABLE GIT_LOCAL_BRANCH)
      string(STRIP "${GIT_LOCAL_BRANCH}" GIT_LOCAL_BRANCH)
      execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GITCOMMAND} config branch.${GIT_LOCAL_BRANCH}.remote
        OUTPUT_VARIABLE GIT_REMOTE)
      string(STRIP "${GIT_REMOTE}" GIT_REMOTE)
      execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GITCOMMAND} config remote.${GIT_REMOTE}.url
        OUTPUT_VARIABLE GIT_REMOTE_URL)
      string(STRIP "${GIT_REMOTE_URL}" GIT_REMOTE_URL)
      add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
        COMMAND echo \\\#define QGSVERSION \\\"${REVISION}\\\"
                >${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
        COMMAND echo \\\#define QGS_GIT_REMOTE_URL \\\"${GIT_REMOTE_URL}\\\"
                >>${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
        MAIN_DEPENDENCY ${GIT_MARKER}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    endif(MSVC)
  else(GITCOMMAND)
    message(STATUS "git marker, but no git found - version will be unknown")
    set(REVISION "unknown")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
         "#define QGSVERSION \"unknown\"")
  endif(GITCOMMAND)
else(GIT_MARKER)
  set(REVISION "exported")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h
       "#define QGSVERSION \"exported\"")
endif(GIT_MARKER)

add_custom_target(version ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qgsversion.h)

# ##############################################################################
find_package(QGIS REQUIRED)
if(NOT QGIS_FOUND)
  message(SEND_ERROR "QGIS dependency was not found!")
endif(NOT QGIS_FOUND)

message(STATUS "4) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
message(STATUS "4) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")
message(STATUS "4) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
message(STATUS "4) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")

# ##############################################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)

add_custom_target(uninstall "${CMAKE_COMMAND}" -P
                            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# ##############################################################################
# Files
set(IMAGE_FILES
    DataBase/images/add.png
    DataBase/images/back.png
    DataBase/images/configure.png
    DataBase/images/connect.png
    DataBase/images/copy.png
    DataBase/images/cut.png
    DataBase/images/dbgrun.png
    DataBase/images/down.png
    DataBase/images/logo_icon.png
    DataBase/images/logo_icon.xcf
    DataBase/images/new.png
    DataBase/images/next.png
    DataBase/images/open.png
    DataBase/images/paste.png
    DataBase/images/previous.png
    DataBase/images/rap_logo.png
    DataBase/images/rap_logo.xcf
    DataBase/images/reload.png
    DataBase/images/remove.png
    DataBase/images/save.png
    DataBase/images/up.png)

set(XML_FILES DataBase/settings.xml DataBase/structure.xml)

set(qrap_SRCS
    qrap.cpp
    cMouseEvents.cpp
    DataBase/cDatabase.cpp
    DataBase/MainWindow.cpp
    DataBase/RapTab.cpp
    DataBase/cRapTableTab.cpp
    DataBase/cRapFormBuilder.cpp
    DataBase/RapDbCommunicator.cpp
    DataBase/cAntennaFilesImporter.cpp
    DataBase/cSiteEditTools.cpp
    DataBase/cRapSites.cpp
    DataBase/cFilter.cpp
    DataBase/cRapCells.cpp
    DataBase/cRapInventory.cpp
    DataBase/cRapSupportingTables.cpp
    DataBase/cRapLinks.cpp
    DataBase/cRapServedKeyLocations.cpp
    DataBase/cRapRasterFiles.cpp
    DataBase/cRapMeasurements.cpp
    DataBase/cSearchFormWidgets.cpp
    DataBase/cSearchForm.cpp
    DataBase/AdminWindow.cpp
    DataBase/LoginDialog.cpp
    DataBase/LogDialog.cpp
    DataBase/PreferencesDialog.cpp
    DataBase/ComboDelegate.cpp
    DataBase/FormCombo.cpp
    DataBase/cBase.cpp
    DataBase/cAlert.cpp
    DataBase/cSettings.cpp
    DataBase/cCsvExportDialog.cpp
    DataBase/cCsvImportDialog.cpp
    Prediction/cMemmngr.cpp
    Prediction/cAntpatn.cpp
    Prediction/cBIN.cpp
    Prediction/cBTLPredict.cpp
    Prediction/cCoveragePredict.cpp
    Prediction/cSpectralLink.cpp
    Prediction/cSpectralPlot.cpp
    Prediction/cGDAL.cpp
    Prediction/cGeoP.cpp
    Prediction/cPlotTask.cpp
    Prediction/cProfile.cpp
    Prediction/cClutter.cpp
    Prediction/cPthlssp.cpp
    Prediction/cRaster.cpp
    Prediction/cRasterFileHandler.cpp
    Prediction/cLink.cpp
    Prediction/cLoadFile.cpp
    Prediction/cMeasImportSpace.cpp
    Prediction/cMeasImportCSV.cpp
    Prediction/cMeasAnalysisCalc.cpp
    Prediction/cPosEstimation.cpp
    Interface/cPlaceSite.cpp
    Interface/cConfirmLink.cpp
    Interface/cConfirmPrediction.cpp
    Interface/cConfirmSpectral.cpp
    Interface/cConfirmMultiLink.cpp
    Interface/cSpectralAnalysis.cpp
    Interface/cLinkAnalysis.cpp
    Interface/cColourManager.cpp
    Interface/cDeleteObject.cpp
    Interface/cFilterDialog.cpp
    Interface/cBackup.cpp
    Interface/Optimisation.cpp
    Interface/cRestore.cpp
    Interface/cImportExportMenu.cpp
    Interface/cUpdateToServer.cpp
    Interface/cCreateUpdateFile.cpp
    Interface/cImportUpdateFile.cpp
    Interface/cMeasurementAnalysis.cpp
    Interface/cLoadMeasurements.cpp
    Interface/LoadRasterFiles/cLoadRasterFiles.cpp
    Interface/LoadRasterFiles/cLoadThread.cpp
    Interface/LoadRasterFiles/cLoadFiles.cpp
    Interface/LoadRasterFiles/cCreateDataSet.cpp
    Interface/LoadRasterFiles/cCutSet.cpp
    Interface/LoadRasterFiles/cOrderArray.cpp
    ExportImport/flatexport/exportf.cpp
    ExportImport/flatimport/importf.cpp
    ExportImport/libfcs/fclient.cpp
    ExportImport/libfcs/idresolve.cpp
    ExportImport/backup/backup.cpp
    ExportImport/backup/restore.cpp
    ExportImport/mergedelete/mergedelete.cpp)

set(qrap_PLUGIN_SRCS ${qrap_SRCS})

set(qrap_UIS
    Interface/Site.ui
    Interface/ConfirmLink.ui
    Interface/LinkAnalysis.ui
    Interface/ConfirmPrediction.ui
    Interface/LoadMeasurements.ui
    Interface/MeasurementAnalysis.ui
    Interface/LoadRasterFiles/CreateDataSet.ui
    Interface/LoadRasterFiles/CutSet.ui
    Interface/LoadRasterFiles/LoadFiles.ui
    Interface/LoadRasterFiles/LoadRasterFiles.ui
    Interface/LinkAnalysis.ui
    Interface/Optimisation.ui
    Interface/ConfirmSpectral.ui
    Interface/ConfirmMultiLink.ui
    Interface/ColourManager.ui
    Interface/DeleteObject.ui
    Interface/FilterDialog.ui
    DataBase/Filter.ui)

set(qrap_MOC_HDRS
    qrap.h
    cMouseEvents.h
    DataBase/cDatabase.h
    DataBase/MainWindow.h
    DataBase/RapTab.h
    DataBase/cRapTableTab.h
    DataBase/cRapFormBuilder.h
    DataBase/RapDbCommunicator.h
    DataBase/cAntennaFilesImporter.h
    DataBase/cRapSites.h
    DataBase/cFilter.h
    DataBase/cRapCells.h
    DataBase/cRapInventory.h
    DataBase/cRapSupportingTables.h
    DataBase/cRapLinks.h
    DataBase/cRapServedKeyLocations.h
    DataBase/cRapRasterFiles.h
    DataBase/cRapMeasurements.h
    DataBase/cSearchFormWidgets.h
    DataBase/cSearchForm.h
    DataBase/AdminWindow.h
    DataBase/LoginDialog.h
    DataBase/LogDialog.h
    DataBase/PreferencesDialog.h
    DataBase/ComboDelegate.h
    DataBase/FormCombo.h
    DataBase/cBase.h
    DataBase/cAlert.h
    DataBase/cSettings.h
    DataBase/Types.h
    DataBase/cCsvExportDialog.h
    DataBase/cCsvImportDialog.h
    DataBase/cSiteEditTools.h
    Prediction/cMemmngr.h
    Prediction/cAntpatn.h
    Prediction/cBIN.h
    Prediction/cBTLPredict.h
    Prediction/cCoveragePredict.h
    Prediction/cSpectralLink.h
    Prediction/cSpectralPlot.h
    Prediction/cGDAL.h
    Prediction/cGeoP.h
    Prediction/cPlotTask.h
    Prediction/cProfile.h
    Prediction/cClutter.h
    Prediction/cPthlssp.h
    Prediction/cRaster.h
    Prediction/cRasterFileHandler.h
    Prediction/PredStructs.h
    Prediction/IntMatrixCalc.h
    Prediction/cLink.h
    Prediction/cLoadFile.h
    Prediction/cMeasImportSpace.h
    Prediction/cMeasImportCSV.h
    Prediction/cMeasAnalysisCalc.h
    Prediction/cPosEstimation.h
    Interface/cPlaceSite.h
    Interface/cConfirmLink.h
    Interface/cConfirmPrediction.h
    Interface/cConfirmSpectral.h
    Interface/cConfirmMultiLink.h
    Interface/cSpectralAnalysis.h
    Interface/cLinkAnalysis.h
    Interface/cFilterDialog.h
    Interface/cColourManager.h
    Interface/cDeleteObject.h
    Interface/cBackup.h
    Interface/cRestore.h
    Interface/cImportExportMenu.h
    Interface/cUpdateToServer.h
    Interface/cCreateUpdateFile.h
    Interface/cImportUpdateFile.h
    Interface/Optimisation.h
    Interface/cMeasurementAnalysis.h
    Interface/cLoadMeasurements.h
    Interface/LoadRasterFiles/cLoadRasterFiles.h
    Interface/LoadRasterFiles/cLoadThread.h
    Interface/LoadRasterFiles/cLoadFiles.h
    Interface/LoadRasterFiles/cCreateDataSet.h
    Interface/LoadRasterFiles/cCutSet.h
    Interface/LoadRasterFiles/cOrderArray.h
    ExportImport/flatexport/exportf.h
    ExportImport/flatimport/importf.h
    ExportImport/libfcs/fclient.h
    ExportImport/libfcs/idresolve.h
    ExportImport/backup/backup.h
    ExportImport/backup/restore.h
    ExportImport/mergedelete/mergedelete.h)

set(qrap_PLUGIN_MOC_HDRS ${QRap_MOC_HDR})

set(qrap_RCCS qrap.qrc DataBase/application.qrc)

install(CODE "MESSAGE(${CMAKE_CURRENT_BINARY_DIR})")

# ##############################################################################
# Build

qt4_wrap_ui(qrap_UIS_H ${qrap_UIS})

qt4_wrap_cpp(qrap_MOC_SRCS ${qrap_MOC_HDRS})

qt4_add_resources(qrap_RCC_SRCS ${qrap_RCCS})

add_library(qrapplugin MODULE ${qrap_SRCS} ${qrap_MOC_SRCS} ${qrap_RCC_SRCS}
                              ${qrap_UIS_H})

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QGIS_INCLUDE_DIR}
  ${GDAL_INCLUDE_DIR}
  ${PROJ_INCLUDE_DIR}
  ${POSTGRES_INCLUDE_DIR}
  ${QT_INCLUDES}
  /usr/include
  /usr/include/c++
  /usr/include/qgis
  /usr/include/eigen3
  /usr/include/gdal
  /usr/include/Poco/
  # /usr/include/qwt-qt4
  /usr/include/qwt
  /usr/include/pqxx
  /usr/include/armadillo_bits
  /usr/include/boost)

target_link_libraries(
  qrapplugin
  ${QGIS_CORE_LIBRARY}
  ${QGIS_GUI_LIBRARY}
  ${POSTGRES_LIBRARY}
  ${QT_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTSVG_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTSQL_LIBRARY}
  libpqxx.so
  libPocoXML.so
  libqgis_core.so
  libqgis_gui.so
  # libqwt-qt4.so
  libqwt.so)

# ##############################################################################
# Install
install(CODE "MESSAGE(\"Installing core...\")")
install(
  TARGETS qrapplugin
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

install(CODE "MESSAGE(\"Installing Settings Files...\")")
install(FILES ${XML_FILES} DESTINATION ${QGIS_PLUGIN_DIR})

install(CODE "MESSAGE(\"Installing Images...\")")
install(FILES ${IMAGE_FILES} DESTINATION ${QGIS_PLUGIN_DIR}/images)
