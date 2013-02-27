//=======================================================================================
//  Headerfiles
//=======================================================================================

// Qt 
#include <QtGui/QtGui>
#include <QtGui/QMdiArea>

// OpenSceneGraph
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Depth>
#include <osgDB/Registry>
#include <osgUtil/Optimizer>
#include <osg/ClipPlane>
#include <osg/ClipNode>
#include <osgUtil/Simplifier>
#include <osgUtil/GLObjectsVisitor>

// Project
#include "MainWindow.h"
#include "OsgWidget.h"
#include "Undo/Commands.h"
#include "BuildQtTreeView.h"
#include "TreeViewItem.h"
#include "SelectionManager.h"
#include "BackgroundGeode.h"
#include "ComputeNodeInfoVisitor.h"
#include "colorlisteditor.h"
#include "test_editor.hpp"

//=======================================================================================
//  Namespace
//=======================================================================================

using namespace std;

namespace QosgSceneEdit 
{

//=======================================================================================

class MyItemDelegate : public QItemDelegate
{
public:
    MyItemDelegate(QObject* parent = 0) : QItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option = QStyleOptionViewItem(), const QModelIndex& index = QModelIndex()) const
    {
        // allow only specific column to be edited, second column in this example
        if (index.column() == 0)
        {
            test_editor* te = new test_editor;
            te->setWindowTitle("Form1");
            return te;//QItemDelegate::createEditor(parent, option, index);
        }
        return 0;
    }
};

class MyItemDelegate2 : public QItemDelegate
{
public:
    MyItemDelegate2(QObject* parent = 0) : QItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option = QStyleOptionViewItem(), const QModelIndex& index = QModelIndex()) const
    {
        // allow only specific column to be edited, second column in this example
        if (index.column() == 0)
        {
            test_editor* te = new test_editor;
            te->setWindowTitle("Form2");
            return te;//QItemDelegate::createEditor(parent, option, index);
        }
        return 0;
    }
};


MainWindow::MainWindow(QWidget* parent, Qt::WFlags flags) 
    : QMainWindow(parent, flags)
    , _lastDirectory(QDir::homePath())
    , _backgroundGeode(new BackgroundGeode())
{
    /*
    QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *colorListCreator = new QStandardItemEditorCreator<test_editor>();
    factory->registerEditor(QVariant::Color, colorListCreator);
    QItemEditorFactory::setDefaultFactory(factory);
    */

    _selectionManager = SelectionManager::instance();

    setupUi(this);

    osg::setNotifyLevel(osg::NOTICE);

    _undoStack = new QUndoStack();

    _scene = new osg::Group();
    _scene->setName("Scene");
    _modelRoot = new osg::Group();
    _modelRoot->setName("ModelRoot");
    _modelRoot->setUserData(_scene.get());
    _internalRoot = new osg::Group();
    _internalRoot->setName("InternalRoot");

    _scene->addChild(_modelRoot.get());
    _scene->addChild(_internalRoot.get());

    QGLFormat format;
    format.setDirectRendering(true);
    format.setRgba(true);
    format.setSwapInterval(1);
    format.setSampleBuffers(true);
    format.setDoubleBuffer(true);
    format.setDepth(true);
    format.setDepthBufferSize(16);

    _viewerWindow = new ViewerQT(format, 0, 0, 0/*, Qt::FramelessWindowHint|Qt::Popup*/);
    _viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
    _viewerWindow->addEventHandler(new osgViewer::StatsHandler);
    _viewerWindow->setSceneData(_scene.get());

    _mdiArea = new QMdiArea(widget);
    _mdiArea->setBackground(QBrush(QPixmap(":/Images/se.png")));

    QBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(_mdiArea);

    QMdiSubWindow *subWindow = _mdiArea->addSubWindow(_viewerWindow);
    subWindow->setGeometry(10, 10, 200, 200);
    subWindow->showMaximized();
    subWindow->setWindowTitle("Render window");

    QToolButton *btn = new QToolButton(_peContainer);
    btn->setIcon(QIcon(":/QosgSceneEdit/QtExt/images/back.png"));
    btn->setIconSize(QSize(12,12));

    layout = new QVBoxLayout(_peContainer);
    //layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);
    layout->addWidget(btn);

    //_reflectionManager = new ReflectionManager(_treeWidget, pe);

    //PropertyCollection *collections = new PropertyCollection("<root>");
    //QVariant value = QString("Test name");
    //IProperty *p = new StringProperty(qvariant_cast<QString>(value), "Name");
    //p->setHasReset(true);
    //collections->addProperty(new SeparatorProperty(QString(), "osg::Object"));
    //collections->addProperty(p);

    //pe->setInitialInput(collections);


    connect(actionNew,  SIGNAL(triggered()), this, SLOT(fileNew()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionUndo, SIGNAL(triggered()), _undoStack, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), _undoStack, SLOT(redo()));
    connect(actionHome, SIGNAL(triggered()), this, SLOT(home()));
    connect(actionZoom, SIGNAL(triggered()), this, SLOT(zoomToSelected()));

    connect(_undoStack, SIGNAL(canUndoChanged(bool)), actionUndo, SLOT(setEnabled(bool)));
    connect(_undoStack, SIGNAL(canRedoChanged(bool)), actionRedo, SLOT(setEnabled(bool)));
    connect(_undoStack, SIGNAL(indexChanged(int)), this, SLOT(undoRedoChanged(int)));
    connect(_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(treeItemSelectionChanged()));
    connect(_treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChanged(QTreeWidgetItem*,int)));


    osg::MatrixTransform* modelview_abs = new osg::MatrixTransform;
    modelview_abs->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    modelview_abs->setMatrix(osg::Matrix::identity());
    modelview_abs->addChild(_backgroundGeode.get());

    osg::Projection* projection = new osg::Projection;
    projection->setMatrix(osg::Matrix::ortho2D(-1,1,-1,1));
    projection->addChild(modelview_abs);
    _internalRoot->addChild(projection);

    fileNew(false);
    treeItemSelectionChanged();
    updateTree();

    //_treeWidget->setItemDelegate(new MyItemDelegate(_treeWidget));
    _treeWidget->setItemDelegateForRow(0, new MyItemDelegate(_treeWidget));
    _treeWidget->setItemDelegateForRow(1, new MyItemDelegate2(_treeWidget));
    _treeWidget->setItemDelegateForColumn(1, new MyItemDelegate2(_treeWidget));

    showMaximized();
}

//=======================================================================================

MainWindow::~MainWindow()
{
}

//=======================================================================================

void MainWindow::fileNew(bool addToHistory)
{
    QUndoCommand *cl = new QUndoCommand(tr("File new"));

    for (unsigned int i=0; i<_modelRoot->getNumChildren(); ++i)
    {
        new RemoveChildCommand(_modelRoot->getChild(i), _modelRoot.get(), cl);
    }

    if (addToHistory)
        _undoStack->push(cl);
    else
    {
        cl->redo();
        delete cl;
    }
}

//=======================================================================================

void MainWindow::fileOpen()
{
    QFileDialog dialog(this, "Open", _lastDirectory);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setFilter(tr("Geometry files (*.osgb *.osgt *.fhs *.flt *.ive *.osg *.3ds *.ac *.dw *.geo *.logo *.lws *.lwo *.md2 *.obj *.stl *.txf *.x)"));
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);
        QFileInfo fileInfo(fileName);

        _lastDirectory = fileInfo.absolutePath();

        if (fileInfo.exists())
        {
            osg::ref_ptr<osg::Node> node;

            emit fileNew();

            node = osgDB::readNodeFile(fileName.toStdString());

            if (node.valid())
            {
                // optimize the scene graph, remove redundant nodes and state etc.
                osgUtil::Optimizer optimizer;
                optimizer.optimize(node.get());

                //osgUtil::GLObjectsVisitor dlVisitor;
                //node->accept(dlVisitor);
            }
            else
            {
                // TODO: Display a messagebox to show error
            }
            
            if (node.valid())
            {
                _undoStack->push(new AddChildCommand(node.get(), _modelRoot.get()));
                _viewerWindow->home();
                //updateTree();
            }
        }
    }
}

//=======================================================================================

void MainWindow::fileSave()
{
    updateTree();
    //osgDB::writeNodeFile(*_scene, fileName.toStdString());
}

//=======================================================================================

void MainWindow::fileSaveAs()
{
    QFileDialog dialog(this, 0, _lastDirectory);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setFilter(tr("Geometry files (*.ive *.osg)"));
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec())
    {
        QString fileName = dialog.selectedFiles().at(0);

        osgDB::writeNodeFile(*_modelRoot->getChild(0), fileName.toStdString());
    }
}

//=======================================================================================

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_undoStack->canUndo())
    {
        if (QMessageBox::question(this, tr("Confirm"), tr("Scene has changed. Do you really want to quit?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            shutdown();
            event->accept();
        }
        else
            event->ignore();
    }
    else
        shutdown();
}

//=======================================================================================

void MainWindow::updateTree()
{
    QosgSceneEdit::BuildQtTreeView visitor(_modelRoot.get());
    _modelRoot->accept(visitor);

    _treeSlider->setMaximum(visitor.getMaxTreeDepth());
    _treeSlider->setValue(visitor.getMaxTreeDepth());

    _treeWidget->clear();
    _treeWidget->addTopLevelItem(visitor.getRoot());
    _treeWidget->expandAll();
    _treeWidget->resizeColumnToContents(0);
}

//=======================================================================================

void MainWindow::undoRedoChanged(int idx)
{
    updateTree();
}

//=======================================================================================

void MainWindow::home()
{
    _viewerWindow->getCameraManipulator()->setNode(_modelRoot.get());
    _viewerWindow->getCameraManipulator()->computeHomePosition();
    _viewerWindow->home();
}

//=======================================================================================

void MainWindow::zoomToSelected()
{
    QList<QTreeWidgetItem*> items = _treeWidget->selectedItems();

    if (!items.empty())
    {
        osg::BoundingSphere sphere = calculateBoundingSphere(items);

        // compute and set new home position
        _viewerWindow->getCameraManipulator()->setHomePosition(sphere.center() + osg::Vec3(0.0f, -3.5f * sphere._radius, 0.0f),
                                                               sphere.center(),
                                                               osg::Vec3(0.0f,0.0f,1.0f));
        _viewerWindow->home();
    }
}

//=======================================================================================

void MainWindow::treeItemSelectionChanged()
{
    QList<QTreeWidgetItem*> items = _treeWidget->selectedItems();

    _selectionManager->clearSelection();

    if (!items.empty())
    {
        if (items.size() == 1)
        {
            TreeViewItem *item = dynamic_cast<TreeViewItem*>(items.front());

            if (item)
            {
                osg::ref_ptr<osg::Node> node = item->getOsgNode();

                if (node.valid())
                {
                    QString minString, maxString;
                    QString name = QString::fromStdString(node->getName());
                    osg::BoundingBox bbox;

                    bbox.expandBy(node->getBound());

                    if (bbox.valid())
                    {
                        minString = QString("(%1 %2 %3)").arg(bbox.xMin(), 0, 'f', 3)
                                .arg(bbox.yMin(), 0, 'f', 3)
                                .arg(bbox.zMin(), 0, 'f', 3);
                        maxString = QString("(%1 %2 %3)").arg(bbox.xMax(), 0, 'f', 3)
                                .arg(bbox.yMax(), 0, 'f', 3)
                                .arg(bbox.zMax(), 0, 'f', 3);
                    }

                    QosgSceneEdit::ComputeNodeInfoVisitor niv;
                    node->accept(niv);

                    if (name.isEmpty())
                        _nameLabel->setText(tr("<unnamed>"));
                    else
                        _nameLabel->setText(name);

                    _boundingBoxMinLabel->setText(minString);
                    _boundingBoxMaxLabel->setText(maxString);
                    _trianglesLabel->setText(QString("%1").arg(niv.getNumTriangles()));

                    _selectionManager->select(node.get());
                }
            }
        }
        else
        {
            QString minString, maxString;
            osg::BoundingBox bbox;

            bbox.expandBy(calculateBoundingSphere(items));

            if (bbox.valid())
            {
                minString = QString("(%1 %2 %3)").arg(bbox.xMin(), 0, 'f', 3)
                        .arg(bbox.yMin(), 0, 'f', 3)
                        .arg(bbox.zMin(), 0, 'f', 3);
                maxString = QString("(%1 %2 %3)").arg(bbox.xMax(), 0, 'f', 3)
                        .arg(bbox.yMax(), 0, 'f', 3)
                        .arg(bbox.zMax(), 0, 'f', 3);
            }

            _nameLabel->setText(QString(tr("<%1 Nodes>")).arg(items.count()));
            _boundingBoxMinLabel->setText(minString);
            _boundingBoxMaxLabel->setText(maxString);

            //foreach (QTreeWidgetItem *tmpItem, items)
            //{
            //	TreeViewItem *item = dynamic_cast<TreeViewItem*>(tmpItem);

            //	if (item)
            //		_selectionManager->select(item->getOsgNode());
            //}
        }
    }
    else
    {
        _nameLabel->setText("");
        _trianglesLabel->setText("");
        _textureMemoryLabel->setText("");
        _boundingBoxMinLabel->setText("");
        _boundingBoxMaxLabel->setText("");
    }
}

//=======================================================================================

osg::BoundingSphere MainWindow::calculateBoundingSphere(const QList<QTreeWidgetItem*>& items)
{
    osg::BoundingSphere sphere;

    // calculate BoundingSphere for all selected nodes
    foreach (QTreeWidgetItem *tmpItem, items)
    {
        TreeViewItem *item = dynamic_cast<TreeViewItem*>(tmpItem);

        if (item)
        {
            osg::ref_ptr<osg::Node> node = item->getOsgNode();

            if (node.valid())
            {
                sphere.expandBy(node->getBound());
            }
        }
    }

    return sphere;
}

//=======================================================================================

void MainWindow::treeItemChanged(QTreeWidgetItem *item_, int column)
{
    TreeViewItem *item = dynamic_cast<TreeViewItem*>(item_);

    if (item)
    {
        osg::ref_ptr<osg::Node> node = item->getOsgNode();

        if (node.valid())
        {
            // TODO: Create new command for setNodeMask
            if (item->checkState(0) == Qt::Unchecked)
                node->setNodeMask(0);
            else
                node->setNodeMask(1);
        }

        for (int i=0; i<item->childCount(); ++i)
        {
            item->child(i)->setCheckState(0, item->checkState(0));
        }
    }
}

//=======================================================================================

void MainWindow::shutdown()
{}

//=======================================================================================

osg::Camera* MainWindow::createHUD()
{
    // create a camera to set up the projection and model view matrices, and the subgraph to drawn in the HUD
    osg::Camera* camera = new osg::Camera;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(-1,1,-1,1));

    // set the view matrix
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::Camera::POST_RENDER);

    // we don't want the camera to grab event focus from the viewers main camera(s).
    camera->setAllowEventFocus(false);

    osg::PositionAttitudeTransform *xform = new osg::PositionAttitudeTransform();
    xform->setScale(osg::Vec3(0.1, 0.1, 0.1));
    xform->setPosition(osg::Vec3d(-0.8, -0.8, 0.0));
    camera->addChild(xform);

    osg::Node *node = osgDB::readNodeFile("D:\\dump\\OpenSceneGraph-Data\\axes.osg");

    if (node)
        xform->addChild(node);

    return camera;
}

//=======================================================================================

void MainWindow::keyPressEvent( QKeyEvent* event )
{
    if (event->key() == Qt::Key_F)
    {
        osgUtil::Simplifier v(0.5);
        _modelRoot->accept(v);
    }

    QMainWindow::keyPressEvent(event);
}

//=======================================================================================

void MainWindow::displayPrompt()
{
}

//=======================================================================================
//  Namespace
//=======================================================================================

} // namespace SceneEdit

//=======================================================================================
