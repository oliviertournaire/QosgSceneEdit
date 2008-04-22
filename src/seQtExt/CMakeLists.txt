INCLUDE(${QT_USE_FILE})
INCLUDE(${OSG_USE_FILE})

SET(LIB_NAME seQtExt)

SET(qtext_SRCS
    QDebugStream.h
    QDebugStream.cpp
    Shell.cpp
    Shell.h
	PropertyEditor/FlagBox.cpp
	PropertyEditor/FlagBox.h
	PropertyEditor/FlagBoxModel.cpp
	PropertyEditor/FlagBoxModel.h
	PropertyEditor/PropertyEditorDelegate.cpp
	PropertyEditor/PropertyEditorDelegate.h
	PropertyEditor/PropertyEditorGlobal.h
	PropertyEditor/PropertyEditorItems.cpp
	PropertyEditor/PropertyEditorItems.h
	PropertyEditor/PropertyEditorModel.cpp
	PropertyEditor/PropertyEditorModel.h
	PropertyEditor/QDesignerColorButton.cpp
	PropertyEditor/QDesignerColorButton.h
	PropertyEditor/QDesignerDefs.cpp
	PropertyEditor/QDesignerDefs.h
	PropertyEditor/QDesignerIconLoader.cpp
	PropertyEditor/QDesignerIconLoader.h
	PropertyEditor/QDesignerLongLongValidator.cpp
	PropertyEditor/QDesignerLongLongValidator.h
	PropertyEditor/QDesignerPropertyLineEdit.cpp
	PropertyEditor/QDesignerPropertyLineEdit.h
	PropertyEditor/QDesignerResourceMimeData.cpp
	PropertyEditor/QDesignerResourceMimeData.h
	PropertyEditor/QDesignerStringListEditor.cpp
	PropertyEditor/QDesignerStringListEditor.h
	PropertyEditor/QDesignerStringListEditorButton.cpp
	PropertyEditor/QDesignerStringListEditorButton.h
	PropertyEditor/QDesignerStyleSheetEditor.cpp
	PropertyEditor/QDesignerStyleSheetEditor.h
	PropertyEditor/QDesignerTextPropertyEditor.cpp
	PropertyEditor/QDesignerTextPropertyEditor.h
	PropertyEditor/QDesignerUtils.h
	PropertyEditor/QPropertyEditor.cpp
	PropertyEditor/QPropertyEditor.h
	PropertyEditor/SharedEnums.h
	PropertyEditor/SharedGlobal.h
)

SET(qtext_MOC_HDRS
    QDebugStream.h
    Shell.h
	PropertyEditor/QPropertyEditor.h
	PropertyEditor/PropertyEditorModel.h
	PropertyEditor/PropertyEditorItems.h
	PropertyEditor/FlagBox.h
	PropertyEditor/FlagBoxModel.h
	PropertyEditor/PropertyEditorDelegate.h
	PropertyEditor/QDesignerResourceMimeData.h
	PropertyEditor/QDesignerTextPropertyEditor.h
	PropertyEditor/QDesignerPropertyLineEdit.h
	PropertyEditor/QDesignerColorButton.h
	PropertyEditor/QDesignerLongLongValidator.h
	PropertyEditor/QDesignerStringListEditorButton.h
	PropertyEditor/QDesignerStringListEditor.h
	PropertyEditor/QDesignerStyleSheetEditor.h
)

SET(qtext_UI
	PropertyEditor/stringlisteditor.ui
)

SOURCE_GROUP("PropertyEditor\\Sources" FILES
	PropertyEditor/FlagBox.cpp
	PropertyEditor/FlagBoxModel.cpp
	PropertyEditor/PropertyEditorDelegate.cpp
	PropertyEditor/PropertyEditorItems.cpp
	PropertyEditor/PropertyEditorModel.cpp
	PropertyEditor/QDesignerColorButton.cpp
	PropertyEditor/QDesignerDefs.cpp
	PropertyEditor/QDesignerIconLoader.cpp
	PropertyEditor/QDesignerLongLongValidator.cpp
	PropertyEditor/QDesignerPropertyLineEdit.cpp
	PropertyEditor/QDesignerResourceMimeData.cpp
	PropertyEditor/QDesignerStringListEditor.cpp
	PropertyEditor/QDesignerStringListEditorButton.cpp
	PropertyEditor/QDesignerStyleSheetEditor.cpp
	PropertyEditor/QDesignerTextPropertyEditor.cpp
	PropertyEditor/QPropertyEditor.cpp
)

SOURCE_GROUP("PropertyEditor\\Headers" FILES
	PropertyEditor/FlagBox.h
	PropertyEditor/FlagBoxModel.h
	PropertyEditor/PropertyEditorDelegate.h
	PropertyEditor/PropertyEditorGlobal.h
	PropertyEditor/PropertyEditorItems.h
	PropertyEditor/PropertyEditorModel.h
	PropertyEditor/QDesignerColorButton.h
	PropertyEditor/QDesignerDefs.h
	PropertyEditor/QDesignerIconLoader.h
	PropertyEditor/QDesignerLongLongValidator.h
	PropertyEditor/QDesignerPropertyLineEdit.h
	PropertyEditor/QDesignerResourceMimeData.h
	PropertyEditor/QDesignerStringListEditor.h
	PropertyEditor/QDesignerStringListEditorButton.h
	PropertyEditor/QDesignerStyleSheetEditor.h
	PropertyEditor/QDesignerTextPropertyEditor.h
	PropertyEditor/QDesignerUtils.h
	PropertyEditor/QPropertyEditor.h
	PropertyEditor/SharedEnums.h
	PropertyEditor/SharedGlobal.h
)

ADD_DEFINITIONS(-DSEQTEXT_EXPORT_LIBRARY)
ADD_DEFINITIONS(-DQT_PROPERTYEDITOR_LIBRARY)
ADD_DEFINITIONS(-DQSCINTILLA_DLL)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

QT4_WRAP_CPP(qtext_MOC_SRCS ${qtext_MOC_HDRS})
QT4_WRAP_UI(qtext_UI_SRCS ${qtext_UI})
QT4_ADD_RESOURCES(qtext_RC_SRCS seQtExt.qrc)

ADD_LIBRARY(seQtExt SHARED ${qtext_SRCS} ${qtext_MOC_SRCS} ${qtext_UI_SRCS} ${qtext_RC_SRCS})
TARGET_LINK_LIBRARIES(seQtExt ${QT_LIBRARIES} ${OSG_LIBRARIES} qscintilla2d)
INSTALL_TARGETS(/bin seQtExt)
