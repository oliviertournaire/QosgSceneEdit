#pragma once

// OpenSceneGraph
#include <osg/NodeVisitor>
#include <QStack>

class TreeViewItem;

class BuildQtTreeView : public osg::NodeVisitor
{
public:

	BuildQtTreeView(osg::Node *node);
	virtual ~BuildQtTreeView();

	virtual void apply(osg::Node& node);

	TreeViewItem* getRoot() { return _parents.top(); }
	int getMaxTreeDepth() const;

private:

	QStack<TreeViewItem*> _parents;
	int _maxTreeDepth;
};
