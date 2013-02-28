#ifndef __OSG_ITEM_DELEGATE_HPP__
#define __OSG_ITEM_DELEGATE_HPP__

#include <QItemDelegate>

#include <osg/Node>
#include <osg/PagedLOD>

#include "pagedlod_editor.hpp"

class osgItemDelegate : public QItemDelegate
{

private:
    typedef enum NodeType
    {
        PAGEDLOD,
        GEODE,
        UNHANDLED_NODETYPE
    } _nodeType;

    _nodeType getNodeTypeFromItemIndex( QModelIndex index, osg::Node* node ) const
    {
        return UNHANDLED_NODETYPE;
    }

public:
    osgItemDelegate(QTreeWidget* parent = 0) : QItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option = QStyleOptionViewItem(), const QModelIndex& index = QModelIndex()) const
    {
        // First, we retrieve the item associated with index
        QTreeWidgetItem *item = (QTreeWidgetItem*)index.internalPointer();
        TreeViewItem* treeitem = dynamic_cast<TreeViewItem*>(item);
        if (treeitem)
        {
            osg::PagedLOD* pl = dynamic_cast<osg::PagedLOD*>(treeitem->getOsgNode());
            if(pl)
            {
                return new pagedlod_editor(pl);
            }
        }
        return 0;
    }
};

#endif // __OSG_ITEM_DELEGATE_HPP__