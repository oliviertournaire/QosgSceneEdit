#include "osgItemDelegate.hpp"

#include <QTreeWidget>

#include <osg/Node>
#include <osg/PagedLOD>

#include "pagedlod_editor.hpp"
#include "TreeViewItem.h"

using namespace osg;

osgItemDelegate::osgItemDelegate(QTreeWidget* parent) : QItemDelegate(parent) {}

QWidget* osgItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // First, we retrieve the item associated with index
    QTreeWidgetItem *item = (QTreeWidgetItem*)index.internalPointer();
    TreeViewItem* treeitem = dynamic_cast<TreeViewItem*>(item);
    if (treeitem)
    {
        PagedLOD* pl = dynamic_cast<PagedLOD*>(treeitem->getOsgNode());
        if(pl)
        {
            return new pagedlod_editor(pl);
        }
    }
    return 0;
}