#include <osg/Node>
#include <osg/Group>

#include "Commands.h"


AddChildCommand::AddChildCommand(osg::Node *child, osg::Group *parent, QUndoCommand *undoParent)
: QUndoCommand("Add child", undoParent)
, _child(child)
, _parent(parent)
{
}

void AddChildCommand::undo()
{
	_parent->removeChild(_child.get());
}

void AddChildCommand::redo()
{
	_parent->addChild(_child.get());
}

RemoveChildCommand::RemoveChildCommand(osg::Node *child, osg::Group *parent, QUndoCommand *undoParent)
: QUndoCommand("Remove child", undoParent)
, _child(child)
, _parent(parent)
{
}

void RemoveChildCommand::redo()
{
	_parent->removeChild(_child.get());
}

void RemoveChildCommand::undo()
{
	_parent->addChild(_child.get());
}
