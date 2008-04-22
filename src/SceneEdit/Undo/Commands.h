#pragma once

// Qt
#include <QUndoCommand>

namespace osg {
	class Node;
	class Group;
}

class AddChildCommand : public QUndoCommand
{
public:

	AddChildCommand(osg::Node *child, osg::Group *parent, QUndoCommand *undoParent = 0L);

	void redo();
	void undo();

private:

	osg::ref_ptr<osg::Node> _child;
	osg::ref_ptr<osg::Group> _parent;
};

class RemoveChildCommand : public QUndoCommand
{
public:

	RemoveChildCommand(osg::Node *child, osg::Group *parent, QUndoCommand *undoParent = 0L);

	void redo();
	void undo();

private:

	osg::ref_ptr<osg::Node> _child;
	osg::ref_ptr<osg::Group> _parent;
};
