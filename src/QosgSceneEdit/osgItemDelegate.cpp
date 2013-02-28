#include "osgItemDelegate.hpp"

#include <QTreeWidget>

#include <osg/Node>
#include <osg/PagedLOD>
#include <osg/MatrixTransform>

#include "editors/pagedlod_editor.hpp"
#include "editors/matrixtransform_editor.hpp"
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
        if(PagedLOD* pl = dynamic_cast<PagedLOD*>(treeitem->getOsgNode()))
            return new pagedlod_editor(pl);
        else if(MatrixTransform* mt = dynamic_cast<MatrixTransform*>(treeitem->getOsgNode()))
            return new matrixtransform_editor(mt);
    }
    return 0;
}
