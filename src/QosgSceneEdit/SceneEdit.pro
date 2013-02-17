CONFIG += qt opengl exceptions thread rtti debug_and_release
QT += opengl
TEMPLATE = app
TARGET = SceneEdit

HEADERS += MainWindow.h \
           OsgWidget.h \
           Undo\Commands.h \
           BuildQtTreeView.h \
		   TreeViewItem.h

SOURCES += main.cpp \
           MainWindow.cpp \
           OsgWidget.cpp \
           Undo\Commands.cpp \
           BuildQtTreeView.cpp \
		   TreeViewItem.cpp

INCLUDEPATH += FhsLoader
HEADERS += FhsLoader/FhsLoader.h FhsLoader/FhsParser.h
SOURCES += FhsLoader/FhsLoader.cpp FhsLoader/FhsLexer.cpp FhsLoader/FhsParser.cpp FhsLoader/ReaderWriterFHS.cpp

FORMS += UI/MainWindow.ui
RESOURCES += resources.qrc

win32 {
    CONFIG += embed_manifest_exe
    HEADERS += Styles/qtdotnetstyle.h
    SOURCES += Styles/qtdotnetstyle.cpp
    INCLUDEPATH += "$(OSG)/include"
    CONFIG(debug, debug|release) {
        LIBS += $(OSG)/lib/osgd.lib $(OSG)/lib/osgGAd.lib $(OSG)/lib/osgViewerd.lib $(OSG)/lib/osgDBd.lib  $(OSG)/lib/osgUtild.lib
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        DESTDIR = build/debug
    } else {
        LIBS += $(OSG)/lib/osg.lib $(OSG)/lib/osgGA.lib $(OSG)/lib/osgViewer.lib $(OSG)/lib/osgDB.lib $(OSG)/lib/osgUtil.lib
        OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        DESTDIR = build/release
    }
}

macx {
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        DESTDIR = build/debug
    } else {
        OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        DESTDIR = build/release
    }
    LIBS += -framework osg \
            -framework osgDB \
            -framework osgGA \
            -framework osgManipulator \
            -framework osgText \
            -framework osgUtil \
            -framework osgViewer \
            -framework OpenThreads
}

unix {
	LIBS += -losg \
			-losgDB \
			-losgGA \
			-losgManipulator \
			-losgUtil \
			-losgViewer \
			-lOpenThreads
}

# install
target.path = ../../bin
INSTALLS += target
