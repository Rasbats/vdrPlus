##---------------------------------------------------------------------------
## Author:      Rasbats aka. Mike Rossiter
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(vdrPlus_pi)

SET(PACKAGE_NAME vdrPlus_pi)
SET(VERBOSE_NAME vdrPlus)
SET(TITLE_NAME vdrPlus)
SET(CPACK_PACKAGE_CONTACT "Mike Rossiter")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")

#SET(CMAKE_BUILD_TYPE Debug)
SET( wxWidgets_USE_DEBUG OFF)
SET( wxWidgets_USE_UNICODE ON)
SET( wxWidgets_USE_UNIVERSAL OFF)
SET( wxWidgets_USE_STATIC OFF)

OPTION(VDR_USE_SVG "Use SVG graphics" ON)

INCLUDE("cmake/PluginConfigure.cmake")

IF(VDR_USE_SVG)
  ADD_DEFINITIONS(-DVDR_USE_SVG)
ENDIF(VDR_USE_SVG)

SET(SRC_vdrPlus
        src/vdrPlus_pi.h
        src/vdrPlus_pi.cpp
        src/icons.h
        src/icons.cpp
	    src/vdrPlusgui.h
	    src/vdrPlusgui.cpp
	    src/vdrPlusgui_impl.cpp
	    src/vdrPlusgui_impl.h
		src/ocpn_plugin.h
		src/folder.xpm
	)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_vdrPlus})

ADD_DEFINITIONS(-DTIXML_USE_STL)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} )

IF(WIN32)
	INSTALL(TARGETS ${PACKAGE_NAME} RUNTIME DESTINATION "plugins")

	SET(INSTALL_DIRECTORY "plugins\\\\${PACKAGE_NAME}")

    IF(EXISTS ${PROJECT_SOURCE_DIR}/data)
		INSTALL(DIRECTORY data DESTINATION "${INSTALL_DIRECTORY}")
    ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/data)

ENDIF(WIN32)


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
