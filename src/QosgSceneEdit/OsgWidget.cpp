// ======================================================================================

#ifndef _WINDOWS
#   include <GL/glu.h>
#else
#   include <Windows.h>
#   include <GL/GLU.h>
#endif // _WINDOWS

#include "OsgWidget.h"

#include <QDesktopWidget>
#include <QMainWindow>

#include <osg/Geode>
#include <osg/Camera>
#include <osg/Geometry>
#include <osg/ClearNode>
#include <osg/Matrix>
#include <osg/Quat>

// ======================================================================================

AdapterWidget::AdapterWidget(const QGLFormat & format, QWidget * parent, 
							 const QGLWidget * shareWidget, WindowFlags f)
: QGLWidget(format, parent, shareWidget, f)
, _fs(false)
{
    _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
	setFocusPolicy(Qt::ClickFocus);
}

// ======================================================================================

void AdapterWidget::resizeGL( int width, int height )
{
    _gw->getEventQueue()->windowResize(0, 0, width, height );
    _gw->resized(0,0,width,height);
}

// ======================================================================================

void AdapterWidget::keyPressEvent( QKeyEvent* event )
{
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );

	//if (event->key() == Qt::Key_F)
	//{
	//	_fs = !_fs;

	//	if (_fs)
	//	{
	//		_oldParent = parentWidget();
	//		_oldSize = size();
	//		this->setParent(0L);
	//		this->move(0,0);
	//		this->resize(QApplication::desktop()->size());
	//		qApp->processEvents();
	//		this->show();
	//	}
	//	else
	//	{
	//		((QMainWindow*)_oldParent)->setCentralWidget(this);
	//		resize(_oldSize);
	//		show();
	//	}
	//}
}

// ======================================================================================

void AdapterWidget::keyReleaseEvent( QKeyEvent* event )
{	
    _gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data()));
}

// ======================================================================================

void AdapterWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;

    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }

    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

// ======================================================================================

void AdapterWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;

    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }

    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

// ======================================================================================

void AdapterWidget::mouseMoveEvent( QMouseEvent* event )
{
    _gw->getEventQueue()->mouseMotion(event->x(), event->y());
}


// ======================================================================================


ViewerQT::ViewerQT(const QGLFormat & format, QWidget * parent, const char * name, 
				   const QGLWidget * shareWidget, WindowFlags f)
: AdapterWidget(format, parent, shareWidget, f)
, _ucsDL(-1)
{
    getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
    getCamera()->setGraphicsContext(getGraphicsWindow());

    setThreadingModel(SingleThreaded);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(10);
}

// ======================================================================================

void ViewerQT::paintGL()
{
    frame();
	//drawUcs();
}

// ======================================================================================

void ViewerQT::drawUcs()
{
	static float _lightDir1[] = {-50.0f, 100.0f, 100.0f, 1.0f};
	static float _lightDir2[] = {100.0f, -25.0f, 100.0f, 1.0f};

    if (_ucsDL < 0)
        CompileUIElements();

	osg::Vec3 axis;
	double angle;

	// Get rotation from matrix as axis/angle rotation.
	getCamera()->getViewMatrix().getRotate().getRotate(angle, axis);

    glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 600.0, 0.0, 600.0, -200.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(50.0f, 50.0f, 100.0f);
    glLightfv(GL_LIGHT0, GL_POSITION, _lightDir1);
    glLightfv(GL_LIGHT1, GL_POSITION, _lightDir2);
	glRotated(angle * 180.0 / osg::PI, axis[0], axis[1], axis[2]);
    DrawUcsIcon();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
}

// ======================================================================================

void ViewerQT::CompileUIElements()
{
    GLUquadric *quadric = gluNewQuadric();

    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);

    _ucsDL = glGenLists(2);

    glNewList(_ucsDL, GL_COMPILE);

    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 1.75, 1.75, 20.0, 16, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 5.0, 0.0, 14.0, 16, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(20.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    gluDisk(quadric, 1.75, 5.0, 16, 1);
    glPopMatrix();

    glEndList();

    glNewList(_ucsDL+1, GL_COMPILE);
    gluSphere(quadric, 4.5, 16, 16);
    glEndList();

    gluDeleteQuadric(quadric);
}

// ======================================================================================

void ViewerQT::DrawUcsIcon()
{
	static float specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
	static float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	static float green[] = {0.0f, 1.0f, 0.0f, 1.0f};
	static float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};

    glPolygonMode(GL_FRONT, GL_FILL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 16);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);

    glPushMatrix();
    glCallList(_ucsDL+1);
    glCallList(_ucsDL);
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glCallList(_ucsDL);
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glCallList(_ucsDL);
    glPopMatrix();
}

// ======================================================================================

void ViewerQT::resizeGL(int width, int height)
{
	AdapterWidget::resizeGL(width,height);

    getCamera()->setProjectionMatrixAsPerspective(45.0f, static_cast<double>(width)/static_cast<double>(height), 1.0f, 10000.0f);
}

// ======================================================================================
