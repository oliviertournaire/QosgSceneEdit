// OSG
#include <osg/Node>

// Project
#include "TreeViewItem.h"

TreeViewItem::TreeViewItem()
: QTreeWidgetItem()
{
    setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

TreeViewItem::TreeViewItem(TreeViewItem *parent, int type)
: QTreeWidgetItem(parent, type)
{
    setFlags( Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

void TreeViewItem::setOsgNode(osg::Node* node)
{
	_node = node;
}

osg::Node* TreeViewItem::getOsgNode()
{
	return _node.get();
}

const osg::Node* TreeViewItem::getOsgNode() const
{
	return _node.get();
}

void TreeViewItem::setOsgObject(osg::Object *obj)
{
	_obj = obj;
}

osg::Object* TreeViewItem::getOsgObject()
{
	return _obj.get();
}

const osg::Object* TreeViewItem::getOsgObject() const
{
	return _obj.get();
}
