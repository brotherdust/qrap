PROJECT(qrap)   

#############################################################
# CMake settings

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.3)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0005 OLD)
ENDIF(COMMAND cmake_policy)

  SET (DEFAULT_BIN_SUBDIR     /usr/bin)
  SET (DEFAULT_LIB_SUBDIR     /usr/lib)
  SET (DEFAULT_DATA_SUBDIR    /usr/share/qgis)
  SET (DEFAULT_PLUGIN_SUBDIR  /usr/lib/qgis/plugins)
  SET (DEFAULT_INCLUDE_SUBDIR /usr/include/qgis)

SET(CMAKE_COLOR_MAKEFILE ON)

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
#SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# it's possible to set PLUGINS_ALSO_BINARIES to TRUE
# then some plugins that can run as standalone apps will be built
# also as standalone apps
SET (PLUGINS_ALSO_BINARIES FALSE)

# try to configure and build GRASS plugin by default
SET (WITH_GRASS FALSE CACHE BOOL "Determines whether GRASS plugin should be built")
SET (GRASS_PREFIX "" CACHE PATH "Path to GRASS base directory")

# try to configure and build python bindings by default
SET (WITH_BINDINGS TRUE CACHE BOOL "Determines whether python bindings should be built")

# By default bindings will be installed only to QGIS directory
# Someone might want to install it to python site-packages directory
# as otherwise user has to use PYTHONPATH environemnt variable to add
# QGIS bindings to package search path
SET (BINDINGS_GLOBAL_INSTALL FALSE CACHE BOOL "Install bindings to global python directory? (might need root)")

# Compile flag. Make it posible to turn it off.
SET (PEDANTIC FALSE CACHE BOOL "Determines if we should compile with -Wall -Werror.")

# whether unit tests should be build
SET (ENABLE_TESTS FALSE CACHE BOOL "Build unit tests?")

# hide this variable because building of python bindings might fail
# if set to other directory than expected
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH)
#############################################################
# check if lexer and parser are not missing
# http://www.mail-archive.com/cmake@cmake.org/msg02861.html

INCLUDE(Flex)

FIND_FLEX()

IF (NOT FLEX_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Couldn't find Flex")
ENDIF (NOT FLEX_EXECUTABLE)

INCLUDE(Bison)

FIND_BISON()

IF (NOT BISON_EXECUTABLE)
  MESSAGE(FATAL_ERROR "Couldn't find Bison")
ENDIF (NOT BISON_EXECUTABLE)

#############################################################
# search for dependencies

# required
FIND_PACKAGE(Proj)

FIND_PACKAGE(Eigen3)
INCLUDE( ${EIGEN3_INCLUDE_DIR} )
 MESSAGE ( "  Found Eigen Include:" ${EIGEN3_INCLUDE_DIR})

#optional since we can use internal sqlite
IF (NOT WITH_INTERNAL_SQLITE3)
  FIND_PACKAGE(Sqlite3)
  IF (NOT SQLITE3_FOUND)
    MESSAGE (SEND_ERROR "sqlite3 dependency was not found (try compiling with sqlite3 internal)!")
  ENDIF (NOT SQLITE3_FOUND)
ENDIF (NOT WITH_INTERNAL_SQLITE3)
FIND_PACKAGE(GEOS)
FIND_PACKAGE(GDAL)

# optional
IF (WITH_POSTGRESQL)
  FIND_PACKAGE(Postgres) # PostgreSQL provider, SPIT plugin
ENDIF (WITH_POSTGRESQL)
FIND_PACKAGE(Expat)    # GPS importer plugin
FIND_PACKAGE(GSL)      # Georeferencer
IF (WITH_GRASS)
  FIND_PACKAGE(GRASS)    # GRASS plugin
ENDIF (WITH_GRASS)
IF (WITH_SPATIALITE)
  FIND_PACKAGE(SPATIALITE)    # SPATIALITE provider
ENDIF (WITH_SPATIALITE)

IF (WITH_BINDINGS)
  # python support:
  # - mapserver export tool
  # - bindings
  INCLUDE (Python) # file cmake/Python.cmake
ENDIF (WITH_BINDINGS)


IF (NOT PROJ_FOUND)
 	MESSAGE (SEND_ERROR "PROJ were not found!")
ENDIF(NOT PROJ_FOUND)
IF (NOT GEOS_FOUND)
	 MESSAGE (SEND_ERROR "GEOS were not found!")
ENDIF (NOT GEOS_FOUND)
IF (NOT GDAL_FOUND)
  MESSAGE (SEND_ERROR "GDAL were not found!")
ENDIF (NOT GDAL_FOUND)

IF (POSTGRES_FOUND)
  # following variable is used in qgsconfig.h
  SET (HAVE_POSTGRESQL TRUE)
ENDIF (POSTGRES_FOUND)

IF (SPATIALITE_FOUND)
  # following variable is used in qgsconfig.h
  SET (HAVE_SPATIALITE TRUE)
ENDIF (SPATIALITE_FOUND)


#############################################################
# search for Qt4

SET( QT_USE_QT3SUPPORT TRUE )
SET( QT_USE_QTXML      TRUE )
SET( QT_USE_QTNETWORK  TRUE )
SET( QT_USE_QTSVG      TRUE )
SET( QT_USE_QTSQL      TRUE )
IF (ENABLE_TESTS)
  SET( QT_USE_QTTEST  TRUE )
  ENABLE_TESTING()
ENDIF (ENABLE_TESTS)

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )

# Disable automatic conversion from QString to ASCII 8-bit strings (char *)
# (Keeps code compatible with Qt/Mac/64bit)
# ADD_DEFINITIONS(-DQT_NO_CAST_TO_ASCII)

FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS ${QT_BINARY_DIR}
    NO_DEFAULT_PATH
    )

###############################################################
FIND_PACKAGE(QGIS REQUIRED)
IF (NOT QGIS_FOUND)
  MESSAGE (SEND_ERROR "QGIS dependency was not found!")
ENDIF (NOT QGIS_FOUND)

#############################################################
# enable warnings

IF (PEDANTIC)
  MESSAGE ("Pedantic compiler settings enabled")
  IF(MSVC)
    ADD_DEFINITIONS( /W4 )

    # disable warnings
    ADD_DEFINITIONS( /wd4100 )  # unused formal parameters
    ADD_DEFINITIONS( /wd4127 )  # constant conditional expressions (used in Qt template classes)
    ADD_DEFINITIONS( /wd4510 )  # default constructor could not be generated (sqlite3_index_info, QMap)
    ADD_DEFINITIONS( /wd4512 )  # assignment operator could not be generated (sqlite3_index_info)
    ADD_DEFINITIONS( /wd4610 )  # user defined constructor required (sqlite3_index_info)
  ELSE (MSVC)
    ADD_DEFINITIONS( -Wall -Wno-long-long -Wformat-security )
    # Qt produces lots of warnings with strict aliasing (as of Qt 4.4.0 & GCC 4.3)
    # ADD_DEFINITIONS( -fstrict-aliasing -Wstrict-aliasing=1 )
  ENDIF (MSVC)
ENDIF (PEDANTIC)

#IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DQGISDEBUG=1)
#ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

#############################################################
# platform specific stuff

set(CMAKE_CXX_FLAGS "-std=c++0x -std=gnu++0x")

IF (WIN32)
  SET (DEFAULT_LIB_SUBDIR     lib)
  SET (DEFAULT_DATA_SUBDIR    .)
  SET (DEFAULT_PLUGIN_SUBDIR  plugins)
  SET (DEFAULT_INCLUDE_SUBDIR include)
  
  IF (MSVC)
    # Python parts need to be build with nmake not vcexpress
    # if you dont add this clause, extra blank msvc projects
    # will pop up mid way through the build process and block
    # the build, plus the python bindings wont build correctly
    SET (CMAKE_MAKE_PROGRAM nmake)
    SET (DEFAULT_BIN_SUBDIR bin)
    # put all the build products into a single directory
    # under build (doesnt affect install target) to make for
    # easier debugging.
    # SET(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    # SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})
    # SET(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR})
    #tell msvc compiler to use main instead of winmain as the
    #application entry point
    #SET(QT_USE_QTMAIN TRUE) 
    # Turn on defines for non standard maths stuff
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)

    # Turn off deprecation warnings
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)
    # Some file access stuff not defined in native win32
    # environment
    ADD_DEFINITIONS(-DF_OK=0)
    ADD_DEFINITIONS(-DX_OK=1)
    ADD_DEFINITIONS(-DW_OK=2)
    ADD_DEFINITIONS(-DR_OK=4)
    
    ADD_DEFINITIONS(-DQGISDEBUG=1)

    INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/win_build/vcdeps/. DESTINATION .)
  ELSE(MSVC)
    SET (DEFAULT_BIN_SUBDIR     .)
  ENDIF(MSVC)
ELSE (WIN32)

  IF (APPLE)
    # for Mac OS X, everything is put inside an application bundle
    SET (CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app/Contents/MacOS)
    # path for library references
    SET (CMAKE_INSTALL_NAME_DIR @executable_path/lib)
    #this will define ${APP_SERVICES_LIBRARY}
    FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
  ENDIF (APPLE)

  # common for MAC and UNIX


ENDIF (WIN32)

IF (WIN32)
  # expect that classes are being imported
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  IF (MSVC)
    ADD_DEFINITIONS("-DCORE_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DGUI_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DPYTHON_EXPORT=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("\"-DCORE_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DGUI_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DPYTHON_EXPORT=__declspec(dllimport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  # other compilers don't use that MSVC construct
  ADD_DEFINITIONS(-DCORE_EXPORT=)
  ADD_DEFINITIONS(-DGUI_EXPORT=)
  ADD_DEFINITIONS(-DPYTHON_EXPORT=)
ENDIF (WIN32)
###############################################################
FIND_PACKAGE(QGIS REQUIRED)
IF (NOT QGIS_FOUND)
  MESSAGE (SEND_ERROR "QGIS dependency was not found!")
ENDIF (NOT QGIS_FOUND)

    MESSAGE(STATUS "1) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
    MESSAGE(STATUS "1) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")

#############################################################
# user-changeable settings which can be used to customize
# layout of QGIS installation
# (default values are platform-specific)
#
#SET (QGIS_BIN_SUBDIR     ${DEFAULT_BIN_SUBDIR}     CACHE STRING "Subdirectory where executables will be installed")
#SET (QGIS_LIB_SUBDIR     ${DEFAULT_LIB_SUBDIR}     CACHE STRING "Subdirectory where libraries will be installed")
#SET (QGIS_DATA_SUBDIR    ${DEFAULT_DATA_SUBDIR}    CACHE STRING "Subdirectory where QGIS data will be installed")
#SET (QGIS_PLUGIN_SUBDIR  ${DEFAULT_PLUGIN_SUBDIR}  CACHE STRING "Subdirectory where plugins will be installed")
#SET (QGIS_INCLUDE_SUBDIR ${DEFAULT_INCLUDE_SUBDIR} CACHE STRING "Subdirectory where header files will be installed")

# mark *_SUBDIR variables as advanced as this is not something
# that an average user would use
#MARK_AS_ADVANCED (QGIS_BIN_SUBDIR QGIS_LIB_SUBDIR QGIS_DATA_SUBDIR QGIS_PLUGIN_SUBDIR QGIS_INCLUDE_SUBDIR)

# full paths for the installation
#SET (QGIS_BIN_DIR     ${QGIS_BIN_SUBDIR})
#SET (QGIS_LIB_DIR     ${QGIS_LIB_SUBDIR})
#SET (QGIS_DATA_DIR    ${QGIS_DATA_SUBDIR})
#SET (QGIS_PLUGIN_DIR  ${QGIS_PLUGIN_SUBDIR})
#SET (QGIS_INCLUDE_DIR ${QGIS_INCLUDE_SUBDIR})

    MESSAGE(STATUS "2) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
    MESSAGE(STATUS "2) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")
	MESSAGE(STATUS "2) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
	MESSAGE(STATUS "2) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")

# manual page - makes sense only on unix systems
IF (UNIX)
  SET (DEFAULT_MANUAL_SUBDIR  man)
  SET (QGIS_MANUAL_SUBDIR  ${DEFAULT_MANUAL_SUBDIR}  CACHE STRING "Subdirectory where manual files will be installed")
  MARK_AS_ADVANCED (QGIS_MANUAL_SUBDIR)
  SET (QGIS_MANUAL_DIR  ${CMAKE_INSTALL_PREFIX}/${QGIS_MANUAL_SUBDIR})
ENDIF (UNIX)

    MESSAGE(STATUS "3) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
    MESSAGE(STATUS "3) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")
	MESSAGE(STATUS "3) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
	MESSAGE(STATUS "3) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")

#############################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_templates/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

    MESSAGE(STATUS "4) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
    MESSAGE(STATUS "4) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")
	MESSAGE(STATUS "4) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
	MESSAGE(STATUS "4) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")

########################################################
# Files
SET (IMAGE_FILES
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
	DataBase/images/up.png
)

SET (XML_FILES
	DataBase/settings.xml
	DataBase/structure.xml
)


SET (qrap_SRCS
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
	ExportImport/mergedelete/mergedelete.cpp
)

SET (qrap_PLUGIN_SRCS
     ${qrap_SRCS}
)

SET (qrap_UIS 
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
	DataBase/Filter.ui
)

SET (qrap_MOC_HDRS
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
	ExportImport/mergedelete/mergedelete.h
)

SET (qrap_PLUGIN_MOC_HDRS
	${QRap_MOC_HDR}
)


SET (qrap_RCCS  qrap.qrc 
        DataBase/application.qrc 
)

	MESSAGE(STATUS "5) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
	MESSAGE(STATUS "5) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")
	MESSAGE(STATUS "5) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
	MESSAGE(STATUS "5) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")

INSTALL(CODE "MESSAGE(${CMAKE_CURRENT_BINARY_DIR})")

########################################################
# Build

QT4_WRAP_UI (qrap_UIS_H  ${qrap_UIS})

QT4_WRAP_CPP (qrap_MOC_SRCS  ${qrap_MOC_HDRS})
 
QT4_ADD_RESOURCES(qrap_RCC_SRCS ${qrap_RCCS})

ADD_LIBRARY (qrapplugin MODULE ${qrap_SRCS}  ${qrap_MOC_SRCS}  ${qrap_RCC_SRCS} ${qrap_UIS_H})

INCLUDE_DIRECTORIES(
      	${CMAKE_CURRENT_BINARY_DIR}
     	${QGIS_INCLUDE_DIR}
     	${GRASS_INCLUDE_DIR}
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
#	/usr/include/libcmaes/src/
	/usr/include/qwt-qt4
#	/usr/include/qwt
  	/usr/include/pqxx
	/usr/include/armadillo_bits
	/usr/include/boost
	../libcmaes/src
	../libcmaes
)

TARGET_LINK_LIBRARIES(qrapplugin
	${QGIS_CORE_LIBRARY}
	${QGIS_GUI_LIBRARY}
	${GRASS_LIBRARIES}
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
	libqwt-qt4.so
#	libqwt.so
)


    MESSAGE(STATUS "6) Found QGIS Plugins Dir: ${QGIS_PLUGIN_DIR}")
	MESSAGE(STATUS "6) Found QGIS Core Library: ${QGIS_CORE_LIBRARY}")
	MESSAGE(STATUS "6) Found QGIS GUI Library: ${QGIS_GUI_LIBRARY}")
    MESSAGE(STATUS "6) Found QGIS Include Dir: ${QGIS_INCLUDE_DIR}")

########################################################
# Install
INSTALL(CODE "MESSAGE(\"Installing core...\")")
INSTALL(TARGETS qrapplugin
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

INSTALL(CODE "MESSAGE(\"Installing Settings Files...\")")
INSTALL(FILES ${XML_FILES}
       DESTINATION ${QGIS_PLUGIN_DIR}
)

      
INSTALL(CODE "MESSAGE(\"Installing Images...\")")
INSTALL(FILES ${IMAGE_FILES}
        DESTINATION ${QGIS_PLUGIN_DIR}/images)


