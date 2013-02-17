#pragma once

// Qt
#include <QTreeWidget>

// OSG
#include <osg/ref_ptr>

namespace osg {
	class Node;
	class Object;
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

	void				setOsgObject(osg::Object *obj);
	osg::Object*		getOsgObject();
	const osg::Object*	getOsgObject() const;


private:

	osg::ref_ptr<osg::Node> _node;
	osg::ref_ptr<osg::Object> _obj;
};
