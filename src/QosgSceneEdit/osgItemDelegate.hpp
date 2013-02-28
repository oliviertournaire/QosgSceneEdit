#ifndef __OSG_ITEM_DELEGATE_HPP__
#define __OSG_ITEM_DELEGATE_HPP__

#include <QItemDelegate>

class QTreeWidget;

namespace osg {
    class Node;
}

class osgItemDelegate : public QItemDelegate
{
public:
    osgItemDelegate(QTreeWidget* parent = 0);
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option = QStyleOptionViewItem(), const QModelIndex& index = QModelIndex()) const;

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
};

#endif // __OSG_ITEM_DELEGATE_HPP__