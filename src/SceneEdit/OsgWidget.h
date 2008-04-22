#pragma once

// OpenSceneGraph
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

// Qt
#include <QString>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QGLWidget>

// System
#include <iostream>
    
using Qt::WindowFlags;

class AdapterWidget : public QGLWidget
{
public:

										AdapterWidget(const QGLFormat & format, QWidget * parent = 0, 
											          const QGLWidget * shareWidget = 0, 
													  Qt::WindowFlags f = 0);

    virtual								~AdapterWidget() {}

    osgViewer::GraphicsWindow*			getGraphicsWindow() { return _gw.get(); }
    const osgViewer::GraphicsWindow*	getGraphicsWindow() const { return _gw.get(); }


protected:

	void			init();

	virtual void	resizeGL( int width, int height );
	virtual void	keyPressEvent( QKeyEvent* event );
	virtual void	keyReleaseEvent( QKeyEvent* event );
	virtual void	mousePressEvent( QMouseEvent* event );
	virtual void	mouseReleaseEvent( QMouseEvent* event );
	virtual void	mouseMoveEvent( QMouseEvent* event );


    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
	bool _fs;
	QSize _oldSize;
	QWidget *_oldParent;
};


class ViewerQT : public osgViewer::Viewer, 
	             public AdapterWidget
{
public:

					ViewerQT(const QGLFormat & format, QWidget * parent = 0, 
							 const char * name = 0, const QGLWidget * shareWidget = 0, 
							 WindowFlags f = 0);

    virtual void	paintGL();
	virtual void	resizeGL(int width, int height);

    
private:

	void			drawUcs();
	void			CompileUIElements();
	void			DrawUcsIcon();


	QTimer _timer;
	int _ucsDL;
	osg::ref_ptr<osg::Camera> _mainCamera;
	osg::ref_ptr<osg::Camera> _preHudCamera;
	osg::ref_ptr<osg::Camera> _postHudCamera;
};
