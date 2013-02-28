//==============================================================================
//  Headerfiles
//==============================================================================

// OSG
#include <osg/Group>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/LOD>
#include <osg/PagedLOD>

// STD
#include <iostream>

// Qt
#include <QTreeWidgetItem>

// Project
#include "BuildQtTreeView.h"
#include "TreeViewItem.h"

//==============================================================================
//  Namespace
//==============================================================================

using namespace QosgSceneEdit;
using namespace std;

//==============================================================================

BuildQtTreeView::BuildQtTreeView(osg::Node *node)
: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
, _maxTreeDepth(0)
{
    TreeViewItem *item = new TreeViewItem();
    item->setText(0, QObject::tr("Root"));
    item->setIcon(0, QIcon(":/icons/TreeView/Group.png"));
	item->setOsgNode(node);
	item->setCheckState(0, Qt::Checked);
    _parents.push(item);
}

//==============================================================================

BuildQtTreeView::~BuildQtTreeView()
{
}

//==============================================================================

void BuildQtTreeView::apply(osg::Node& node)
{
	osg::Geode *geode = 0L;
    bool isGroup = false;
    QString nodeName;
    if(node.className()!="")
        nodeName = "[" + QString::fromStdString(node.className()) + "]";

    if (nodeName.isEmpty())
    {
        if(node.getName()!="")
            nodeName += " " + QString::fromStdString(node.getName());
        if (nodeName.isEmpty())
        {
            nodeName = QObject::tr("[<unknow_type>] <unnamed>");
        }
    }

    TreeViewItem *item = new TreeViewItem(_parents.top());
	item->setCheckState(0, Qt::Checked);
	item->setOsgNode(&node);
    item->setData(0, Qt::DisplayRole, nodeName);

    if (dynamic_cast<osg::PagedLOD*>(&node))
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/LOD.png"));
        _parents.push(item);
        isGroup = true;
    }
    else if (dynamic_cast<osg::LOD*>(&node))
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/LOD.png"));
        _parents.push(item);
        isGroup = true;
    }
	else if (dynamic_cast<osg::Group*>(&node))
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/Group.png"));
        _parents.push(item);
        isGroup = true;
    }
	else if (dynamic_cast<osg::Billboard*>(&node))
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/Billboard.png"));
	}
	else if ((geode = dynamic_cast<osg::Geode*>(&node)))
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/Geode.png"));

		for (unsigned int i=0; i<geode->getNumDrawables(); ++i)
		{
			osg::ref_ptr<osg::Drawable> drawable = geode->getDrawable(i);

			if (drawable.valid())
			{
				TreeViewItem *subItem = new TreeViewItem(item);
				//item->setCheckState(0, Qt::Checked);
				//item->setOsgNode(&node);
				subItem->setOsgObject(drawable.get());
                subItem->setData(0, Qt::DisplayRole, "Drawable");
			}
		}
    }
	else
    {
        item->setData(0, Qt::DecorationRole, QIcon(":/icons/TreeView/Other.png"));
    }

    _maxTreeDepth = std::max<int>(_maxTreeDepth, _parents.size());

    traverse(node);

    if (isGroup)
    {
        _parents.pop();
    }
}

//==============================================================================

int BuildQtTreeView::getMaxTreeDepth() const
{
    return _maxTreeDepth;
}

//==============================================================================
