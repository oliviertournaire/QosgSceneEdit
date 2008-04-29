#pragma once

// OpenSceneGraph
#include <osg/ref_ptr>
#include <osg/BoundingSphere>

// Project
#include "ui_MainWindow.h"

// Forward declaration
class ViewerQT;
class QUndoStack;
class QUndoView;
class QVBoxLayout;
class QMdiArea;

class QDebugStream;
class SelectionManager;
class ReflectionManager;

namespace osg {
	class Group;
	class Camera;
}

namespace SceneEdit {
	class BackgroundGeode;
}

class MainWindow : public QMainWindow,
				   protected Ui_MainWindow
{
	Q_OBJECT

public:

							MainWindow(QWidget* parent = 0, Qt::WFlags flags = 0);
	virtual					~MainWindow();

	osg::BoundingSphere		calculateBoundingSphere(const QList<QTreeWidgetItem*>& items);

	virtual void			keyPressEvent( QKeyEvent* event );


protected:

	void					closeEvent(QCloseEvent *event);
	void					updateTree();


protected slots:

	void					fileNew(bool addToHistory=true);
	void					fileOpen();
	void					fileSave();
	void					fileSaveAs();
	void					undoRedoChanged(int idx);
    void					home();
    void					zoomToSelected();
	void					treeItemSelectionChanged();
	void					treeItemChanged(QTreeWidgetItem *item, int column);

	void					displayPrompt();


private:

	void					shutdown();
	osg::Camera*			createHUD();


	ViewerQT *_viewerWindow;

	osg::ref_ptr<osg::Group> _scene;
	osg::ref_ptr<osg::Group> _modelRoot;
	osg::ref_ptr<osg::Group> _internalRoot;
	osg::ref_ptr<SceneEdit::BackgroundGeode> _backgroundGeode;
	osg::ref_ptr<osg::Camera> _hudCamera;

	QString _lastDirectory;

	QUndoStack *_undoStack;
	QMdiArea *_mdiArea;
	QDebugStream *_debugStream;
	osg::ref_ptr<SelectionManager> _selectionManager;
	osg::ref_ptr<ReflectionManager> _reflectionManager;
};
