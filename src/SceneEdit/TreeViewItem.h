#pragma once

// Qt
#include <QTreeWidget>

// OSG
#include <osg/ref_ptr>

namespace osg {
	class Node;
}

class TreeViewItem :
	public QTreeWidgetItem
{
public:

						TreeViewItem();
						TreeViewItem(TreeViewItem *parent, int type = Type);
	virtual				~TreeViewItem();

	void				setOsgNode(osg::Node* node);
	osg::Node*			getOsgNode();
	const osg::Node*	getOsgNode() const;


private:

	osg::ref_ptr<osg::Node> _node;
};
