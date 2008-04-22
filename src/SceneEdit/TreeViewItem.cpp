// OSG
#include <osg/Node>

// Project
#include "TreeViewItem.h"

TreeViewItem::TreeViewItem()
: QTreeWidgetItem()
{
}

TreeViewItem::TreeViewItem(TreeViewItem *parent, int type)
: QTreeWidgetItem(parent, type)
{
}

TreeViewItem::~TreeViewItem()
{
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
