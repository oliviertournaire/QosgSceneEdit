/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "QPropertyEditor.h"
#include "PropertyEditorModel.h"
#include "PropertyEditorDelegate.h"

#include "QDesignerResourceMimeData.h"

#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QDesignerFormWindowManagerInterface>

#include <QtGui/QHeaderView>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QScrollBar>
#include <QtGui/qevent.h>
#include <qdebug.h>


Q_GLOBAL_STATIC_WITH_ARGS(PropertyCollection, dummy_collection, (QLatin1String("<empty>")))

QPropertyEditor::QPropertyEditor(QWidget *parent)
: QTreeView(parent)
, m_model(new QPropertyEditorModel(this))
, m_itemDelegate(new QPropertyEditorDelegate(this))
{
    setModel(m_model);
    setItemDelegate(m_itemDelegate);
    setTextElideMode (Qt::ElideMiddle);

    connect(header(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(headerDoubleClicked(int)));

    connect(m_itemDelegate, SIGNAL(resetProperty(const QString &)), m_model, SIGNAL(resetProperty(const QString &)));
    setInitialInput(0);

    setAlternatingRowColors(true);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(this, SIGNAL(activated(QModelIndex)),
            this, SLOT(expand(QModelIndex)));

    connect(m_model, SIGNAL(propertyChanged(IProperty*)),
            this, SIGNAL(propertyChanged(IProperty*)));

    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);
}

QPropertyEditor::~QPropertyEditor()
{
}

bool QPropertyEditor::isReadOnly() const
{
    return m_itemDelegate->isReadOnly();
}

void QPropertyEditor::setReadOnly(bool readOnly)
{
    m_itemDelegate->setReadOnly(readOnly);
}

void QPropertyEditor::setInitialInput(IProperty *initialInput)
{
    const int oldColumnWidth  = columnWidth(0);

    QScrollBar *sb = verticalScrollBar();

    const int position = sb->value();
    const bool resizeToColumn = !m_model->initialInput() || m_model->initialInput() == dummy_collection();

    if (!initialInput)
        initialInput = dummy_collection();

    m_model->setInitialInput(initialInput);

    setSelectionMode(QTreeView::SingleSelection);
    setSelectionBehavior(QTreeView::SelectRows);
    setRootIsDecorated(true);

    setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::SelectedClicked);
    setRootIndex(m_model->indexOf(initialInput));

    if (resizeToColumn) {
        resizeColumnToContents(0);
    } else {
        setColumnWidth (0, oldColumnWidth);
    }
    sb->setValue(position);
}

IProperty *QPropertyEditor::initialInput() const
{
    return m_model->initialInput();
}

void QPropertyEditor::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
#if 0
	QTreeView::drawBranches(painter, rect, index);
	return;
#else
    // designer fights the style it uses. :(
    static const bool mac_style = QApplication::style()->inherits("QMacStyle");
    static const int windows_deco_size = 9;

    QStyleOptionViewItem opt = viewOptions();

    IProperty *property = static_cast<const QPropertyEditorModel*>(model())->privateData(index);
    if (index.column() == 0 && property && property->changed()) 
	{
        opt.font.setBold(true);
    }

    if (property && property->isSeparator()) 
	{
        painter->fillRect(rect, opt.palette.dark());
    }

    if (model()->hasChildren(index)) 
	{
        opt.state |= QStyle::State_Children;

        QRect primitive(rect.left(), rect.top(), indentation(), rect.height());

        if (!mac_style) 
		{
            primitive.moveLeft(primitive.left() + (primitive.width() - windows_deco_size)/2);
            primitive.moveTop(primitive.top() + (primitive.height() - windows_deco_size)/2);
            primitive.setWidth(windows_deco_size);
            primitive.setHeight(windows_deco_size);
        }

        opt.rect = primitive;

        if (isExpanded(index))
            opt.state |= QStyle::State_Open;

        style()->drawPrimitive(QStyle::PE_IndicatorBranch, &opt, painter, this);
    }

    const QPen savedPen = painter->pen();
    const QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &opt));
    painter->setPen(QPen(color));
    painter->drawLine(rect.x(), rect.bottom(), rect.right(), rect.bottom());
    painter->setPen(savedPen);
#endif
}

void QPropertyEditor::keyPressEvent(QKeyEvent *ev)
{
/*    QApplication::syncX();*/
    QTreeView::keyPressEvent(ev);
}

QStyleOptionViewItem QPropertyEditor::viewOptions() const
{
    QStyleOptionViewItem option = QTreeView::viewOptions();
    option.showDecorationSelected = true;
    return option;
}

void QPropertyEditor::focusInEvent(QFocusEvent *event)
{
    QAbstractScrollArea::focusInEvent(event);
    viewport()->update();
}

void QPropertyEditor::headerDoubleClicked(int column)
{
    resizeColumnToContents(column);
}

// Check for image
static inline const ResourceMimeData *imageResourceMimeData(const QMimeData *data)
{
    const ResourceMimeData *resourceMimeData = qobject_cast<const ResourceMimeData *>(data);
    if (!resourceMimeData || resourceMimeData->type() != ResourceMimeData::Image)
        return 0;
    return resourceMimeData;
}


void  QPropertyEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if (!isReadOnly() && imageResourceMimeData(event->mimeData()))
        event->acceptProposedAction();
    else
        event->ignore();
}

void  QPropertyEditor::dragMoveEvent(QDragMoveEvent *event)
{
    if (!isReadOnly() && imageResourceMimeData(event->mimeData()))
        event->acceptProposedAction();
    else
        event->ignore();
}

void QPropertyEditor::dropEvent ( QDropEvent * event )
{
    bool accept = false;
    do {
        if (isReadOnly())
            break;

        const ResourceMimeData *image = imageResourceMimeData(event->mimeData());
        if (!image)
            break;

        const QModelIndex index = indexAt(event->pos());
        if (!index.isValid())
            break;

        accept = m_model->resourceImageDropped(index, image);
    } while (false);

    if ( accept) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void QPropertyEditor::mousePressEvent(QMouseEvent *event)
{
    // we want to handle mousePress in EditingState (persistent editors)
    if ((state() != NoState && state() != EditingState) || !viewport()->rect().contains(event->pos())) 
	{
        return;
    }
    //int i = itemDecorationAt(event->pos());
	QModelIndex i = indexAt(event->pos());
	QRect rect = visualRect(i);
    if (i.isValid() && itemsExpandable() && model()->hasChildren(i)/*&& hasChildren(d->viewItems.at(i).index)*/) 
	{
        if (isExpanded(i))
            collapse(i);
        else
            expand(i);

        if (!isAnimated())
		{
            updateGeometries();
            viewport()->update();
        }
    } 
	else 
	{
        QTreeView::mousePressEvent(event);
    }
}

int QPropertyEditor::itemDecorationAt(const QPoint &pos) const
{
	return -1;
 //   int x = pos.x();
 //   int column = header()->logicalIndexAt(x);
 //   if (column == -1)
 //       return -1; // no logical index at x
 //   int position = header()->sectionViewportPosition(column);
 //   int size = header()->sectionSize(column);
 //   int cx = (isRightToLeft() ? size - x + position : x - position);
 //   //int viewItemIndex = itemAtCoordinate(pos.y());
 //   //int itemIndentation = indentationForItem(viewItemIndex);
 //   //QModelIndex index = modelIndex(viewItemIndex);
	//int viewItemIndex = 0;
	//int itemIndentation = 0;
	//QModelIndex index = indexAt(pos);

 //   if (!index.isValid() || column != 0
 //       || cx < (itemIndentation - indentation()) || cx > itemIndentation)
 //       return -1; // pos is outside the decoration rect

 //   //if (!rootDecoration && index.parent() == root)
 //   //    return -1; // no decoration at root

 //   QRect rect;
 //   if (isRightToLeft())
 //       rect = QRect(position + size - itemIndentation, coordinateForItem(viewItemIndex),
 //                    indentation(), itemHeight(viewItemIndex));
 //   else
 //       rect = QRect(position + itemIndentation - indent, coordinateForItem(viewItemIndex),
 //                    indentation(), itemHeight(viewItemIndex));

 //   //QStyleOption opt;
 //   //opt.initFrom(q);
 //   //opt.rect = rect;
 //   //QRect returning = q->style()->subElementRect(QStyle::SE_TreeViewDisclosureItem, &opt, q);
 //   //if (!returning.contains(pos))
 //   //    return -1;

	//if (!rect.contains(pos))
	//	return -1;

 //   return viewItemIndex;
}
