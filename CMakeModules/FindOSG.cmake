# Locate OpenSceneGraph
# This module defines
# OSG_LIBRARY
# OSG_FOUND, if false, do not try to link to gdal 
# OSG_INCLUDE_DIR, where to find the headers
#
# $OSG_DIR is an environment variable that would
#
# Created by Andre Normann. 

SET(OSG_USE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/UseOSG.cmake)

SET(OSG_DEFINITIONS "")

FIND_PATH(OSG_INCLUDE_DIR osg/Version
    $ENV{OSG_DIR}/include
    $ENV{OSG_ROOT}/include
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/include
    /usr/freeware/include
)

MACRO(FIND_OSG_LIBRARY MYLIBRARY MYLIBRARYNAME)
    FIND_LIBRARY(${MYLIBRARY} 
        NAMES ${MYLIBRARYNAME}
        PATHS
        $ENV{OSG_DIR}/lib
        $ENV{OSG_ROOT}/lib
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/lib
        /usr/local/lib64
        /usr/lib
        /sw/lib
        /opt/local/lib
        /opt/csw/lib
        /opt/lib
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSG_ROOT]/lib
        /usr/freeware/lib64
    )
ENDMACRO(FIND_OSG_LIBRARY LIBRARY LIBRARYNAME)

FIND_OSG_LIBRARY(OSG_LIBRARY_RELEASE osg)
FIND_OSG_LIBRARY(OSG_LIBRARY_DEBUG osgd)

FIND_OSG_LIBRARY(OSGGA_LIBRARY_RELEASE osgGA)
FIND_OSG_LIBRARY(OSGGA_LIBRARY_DEBUG osgGAd)

FIND_OSG_LIBRARY(OSGMANIPULATOR_LIBRARY_RELEASE osgManipulator)
FIND_OSG_LIBRARY(OSGMANIPULATOR_LIBRARY_DEBUG osgManipulatord)

FIND_OSG_LIBRARY(OSGDB_LIBRARY_RELEASE osgDB)
FIND_OSG_LIBRARY(OSGDB_LIBRARY_DEBUG osgDBd)

FIND_OSG_LIBRARY(OSGVIEWER_LIBRARY_RELEASE osgViewer)
FIND_OSG_LIBRARY(OSGVIEWER_LIBRARY_DEBUG osgViewerd)

FIND_OSG_LIBRARY(OSGUTIL_LIBRARY_RELEASE osgUtil)
FIND_OSG_LIBRARY(OSGUTIL_LIBRARY_DEBUG osgUtild)

FIND_OSG_LIBRARY(OSGINTROSPECTION_LIBRARY_RELEASE osgIntrospection)
FIND_OSG_LIBRARY(OSGINTROSPECTION_LIBRARY_DEBUG osgIntrospectiond)

MACRO (_OSG_ADJUST_LIB_VARS basename)
    IF (OSG_INCLUDE_DIR)

      # if only the release version was found, set the debug variable also to the release version
      IF (${basename}_LIBRARY_RELEASE AND NOT ${basename}_LIBRARY_DEBUG)
        SET(${basename}_LIBRARY_DEBUG ${${basename}_LIBRARY_RELEASE})
        SET(${basename}_LIBRARY       ${${basename}_LIBRARY_RELEASE})
        SET(${basename}_LIBRARIES     ${${basename}_LIBRARY_RELEASE})
      ENDIF (${basename}_LIBRARY_RELEASE AND NOT ${basename}_LIBRARY_DEBUG)

      # if only the debug version was found, set the release variable also to the debug version
      IF (${basename}_LIBRARY_DEBUG AND NOT ${basename}_LIBRARY_RELEASE)
        SET(${basename}_LIBRARY_RELEASE ${${basename}_LIBRARY_DEBUG})
        SET(${basename}_LIBRARY         ${${basename}_LIBRARY_DEBUG})
        SET(${basename}_LIBRARIES       ${${basename}_LIBRARY_DEBUG})
      ENDIF (${basename}_LIBRARY_DEBUG AND NOT ${basename}_LIBRARY_RELEASE)
      IF (${basename}_LIBRARY_DEBUG AND ${basename}_LIBRARY_RELEASE)
        # if the generator supports configuration types then set
        # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
        IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          SET(${basename}_LIBRARY       optimized ${${basename}_LIBRARY_RELEASE} debug ${${basename}_LIBRARY_DEBUG})
        ELSE(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          # if there are no configuration types and CMAKE_BUILD_TYPE has no value
          # then just use the release libraries
          SET(${basename}_LIBRARY       ${${basename}_LIBRARY_RELEASE} )
        ENDIF(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(${basename}_LIBRARIES       optimized ${${basename}_LIBRARY_RELEASE} debug ${${basename}_LIBRARY_DEBUG})
      ENDIF (${basename}_LIBRARY_DEBUG AND ${basename}_LIBRARY_RELEASE)

      SET(${basename}_LIBRARY ${${basename}_LIBRARY} CACHE FILEPATH "The OSG ${basename} library")

      IF (${basename}_LIBRARY)
        SET(${basename}_FOUND 1)
      ENDIF (${basename}_LIBRARY)

    ENDIF (OSG_INCLUDE_DIR )

    # Make variables changeble to the advanced user
    MARK_AS_ADVANCED(${basename}_LIBRARY ${basename}_LIBRARY_RELEASE ${basename}_LIBRARY_DEBUG OSG_INCLUDE_DIR)
ENDMACRO (_OSG_ADJUST_LIB_VARS)


_OSG_ADJUST_LIB_VARS(OSG)
_OSG_ADJUST_LIB_VARS(OSGGA)
_OSG_ADJUST_LIB_VARS(OSGMANIPULATOR)
_OSG_ADJUST_LIB_VARS(OSGDB)
_OSG_ADJUST_LIB_VARS(OSGVIEWER)
_OSG_ADJUST_LIB_VARS(OSGUTIL)
_OSG_ADJUST_LIB_VARS(OSGINTROSPECTION)


SET(OSG_FOUND "NO")
IF(OSG_LIBRARY AND OSG_INCLUDE_DIR)
    SET(OSG_FOUND "YES")
ENDIF(OSG_LIBRARY AND OSG_INCLUDE_DIR)
