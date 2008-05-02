#pragma once

//==============================================================================
//  Headerfiles
//==============================================================================

#include <osg/NodeVisitor>

#include <QStack>

//==============================================================================
//  Forward declarations
//==============================================================================

class TreeViewItem;

//==============================================================================
//  Namespace
//==============================================================================

namespace SceneEdit {

//==============================================================================
//  Class Declaration
//==============================================================================

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

//==============================================================================
//  Namespace
//==============================================================================

} // namespace SceneEdit

//==============================================================================
