SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTMAIN TRUE)

INCLUDE(${QT_USE_FILE})

#
# Header and source files
#
SET(SE_SOURCES
	BuildQtTreeView.h
	BuildQtTreeView.cpp
	main.cpp
	MainWindow.h
	MainWindow.cpp
	OsgWidget.h
	OsgWidget.cpp
	TreeViewItem.h
	TreeViewItem.cpp
	Undo/Commands.h
	Undo/Commands.cpp
	Styles/seStyle.h
	Styles/seStyle.cpp
	BackgroundGeode.h
	BackgroundGeode.cpp
	SelectionManager.h
	SelectionManager.cpp
	ReflectionManager.h
	ReflectionManager.cpp
	ComputeNodeInfoVisitor.h
	ComputeNodeInfoVisitor.cpp
)

#
# On Windows enable the .NET-Style
#
#IF(WIN32)
#    SET(SE_SOURCES ${SE_SOURCES} Styles/qtdotnetstyle.cpp)
#ENDIF(WIN32)

#
# Headers which have a Q_OBJECT
#
SET(SE_MOC_HEADERS
	MainWindow.h
	ReflectionManager.h
)

#
# Generate C++ files from ui file
#
SET(SE_UIS
	UI/MainWindow.ui
)

#
# FhsLoader
#
#IF(NOT BUILD_FHS_OSG_PLUGIN)
#    SET(FHSLOADER_LIB FhsLoader)
#    ADD_DEFINITIONS(-DUSE_FHSLOADER)
#ELSE(NOT BUILD_FHS_OSG_PLUGIN)
#    SET(FHSLOADER_LIB "")
#ENDIF(NOT BUILD_FHS_OSG_PLUGIN)

#
# Qt4 setup
#
QT4_WRAP_CPP(SE_MOC_SOURCES ${SE_MOC_HEADERS})
QT4_WRAP_UI(SE_UIS_H ${SE_UIS})
QT4_ADD_RESOURCES(SE_RC_SOURCES resources.qrc)

INCLUDE_DIRECTORIES(
	../FhsLoader 
	../seQtExt 
	../seQtExt/PropertyEditor 
	../seQtExt/qconsole 
	${CMAKE_CURRENT_BINARY_DIR}
)
include_directories( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIR}
)

SET(EXECUTABLE_OUTPUT_PATH ${SE_EXECUTABLE_DIR})

ADD_EXECUTABLE(SceneEdit WIN32 ${SE_SOURCES} ${SE_MOC_SOURCES} ${SE_UIS_H} ${SE_RC_SOURCES})
TARGET_LINK_LIBRARIES(SceneEdit ${OPENSCENEGRAPH_LIBRARIES} ${QT_LIBRARIES} ${OPENGL_LIBRARIES} )

INSTALL(
    TARGETS SceneEdit
    RUNTIME DESTINATION bin
)
