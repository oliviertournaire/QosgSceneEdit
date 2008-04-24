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

#include "PropertyEditorDelegate.h"
#include "PropertyEditorModel.h"
#include "QDesignerTextPropertyEditor.h"
#include "QDesignerIconLoader.h"

#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>
#include <QtGui/QToolButton>
#include <QtGui/QHBoxLayout>

#include <QtCore/qdebug.h>



// ================================================================================================
// === EditorWithReset
// ================================================================================================

EditorWithReset::EditorWithReset(const IProperty *property, QPropertyEditorModel *model, QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    m_property = property;
    m_child_editor = 0;
    m_layout = new QHBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_model = model;

    QToolButton *button = new QToolButton(this);
    button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    button->setIcon(createIconSet(QLatin1String("resetproperty.png")));
    button->setIconSize(QSize(8,8));
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    m_layout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(emitResetProperty()));
}

// ================================================================================================

void EditorWithReset::emitResetProperty()
{
    emit resetProperty(m_property, m_model);
}

// ================================================================================================

void EditorWithReset::setChildEditor(QWidget *child_editor)
{
    m_child_editor = child_editor;

    m_child_editor->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
    m_layout->insertWidget(0, m_child_editor);
    setFocusProxy(m_child_editor);
}



// ================================================================================================
// === EditorWithCommitAndRevert
// ================================================================================================

EditorWithCommitAndRevert::EditorWithCommitAndRevert(const IProperty *property, 
													 QPropertyEditorModel *model, 
													 QWidget *parent)
: QWidget(parent)
{
    setAutoFillBackground(true);
    m_property = property;
    m_child_editor = 0;
    m_layout = new QHBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_model = model;

    QToolButton *button = new QToolButton(this);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setIcon(createIconSet(QLatin1String("commit.png")));
    button->setIconSize(QSize(8,8));
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    m_layout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(emitCommitProperty()));

	button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setIcon(createIconSet(QLatin1String("revert.png")));
    button->setIconSize(QSize(8,8));
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
    m_layout->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(emitRevertProperty()));

}

// ================================================================================================

void EditorWithCommitAndRevert::emitCommitProperty()
{
    emit commitProperty(m_property, m_model);
}

// ================================================================================================

void EditorWithCommitAndRevert::emitRevertProperty()
{
    emit revertProperty(m_property, m_model);
}

// ================================================================================================

void EditorWithCommitAndRevert::setChildEditor(QWidget *child_editor)
{
    m_child_editor = child_editor;

    m_child_editor->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
    m_layout->insertWidget(0, m_child_editor);
    setFocusProxy(m_child_editor);
}



// ================================================================================================
// === QPropertyEditorDelegate
// ================================================================================================

QPropertyEditorDelegate::QPropertyEditorDelegate(QObject *parent)
    : QItemDelegate(parent),
      m_readOnly(false)
{
}

// ================================================================================================

QPropertyEditorDelegate::~QPropertyEditorDelegate()
{
}

// ================================================================================================

bool QPropertyEditorDelegate::eventFilter(QObject *object, QEvent *event)
{
    QWidget *editor = qobject_cast<QWidget*>(object);

    if (editor && qobject_cast<EditorWithReset*>(editor->parent()))
        editor = editor->parentWidget();

	if (editor && qobject_cast<EditorWithCommitAndRevert*>(editor->parent()))
		editor = editor->parentWidget();

    switch (event->type()) 
	{
		case QEvent::KeyPress:
		case QEvent::KeyRelease: 
			{
				QKeyEvent *ke = static_cast<QKeyEvent*>(event);

				if (!(ke->modifiers() & Qt::ControlModifier) && 
					(ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down)) 
				{
					event->ignore();
					return true;
				}

				if (ke->key() == Qt::Key_Escape)
				{
					closeEditor(editor);
					return false;
				}
			} 
			break;

		case QEvent::FocusOut:
            if (!editor->isActiveWindow() || (QApplication::focusWidget() != editor)) 
			{
                QWidget *w = QApplication::focusWidget();

                while (w) 
				{ // do not worry about focus changes internally in the editor
                    if (w == editor)
                        return false;

                    w = w->parentWidget();
                }

                emit commitData(editor);
            }
            return false;

        default:
            break;
    }

    return QItemDelegate::eventFilter(editor ? editor : object, event);
}

// ================================================================================================

void QPropertyEditorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &opt, 
									const QModelIndex &index) const
{
    QStyleOptionViewItem option = opt;

    const QAbstractItemModel *model = index.model();
    IProperty *property = static_cast<const QPropertyEditorModel*>(model)->privateData(index);

    if (index.column() == 0 && property && property->changed()) 
	{
        option.font.setBold(true);
    }

    if (property && property->isSeparator()) 
	{
        option.palette.setColor(QPalette::Text, option.palette.color(QPalette::BrightText));
        option.font.setBold(true);
        option.state &= ~QStyle::State_Selected;
    }

    if (index.column() == 1) 
	{
        option.state &= ~QStyle::State_Selected;
    }

    option.state &= ~QStyle::State_HasFocus;

    if (property && property->isSeparator()) 
	{
        const QBrush bg = option.palette.dark();
        painter->fillRect(option.rect, bg);
    }

    const QPen savedPen = painter->pen();

    QItemDelegate::paint(painter, option, index);

    const QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));

    painter->setPen(QPen(color));

    if (index.column() == 1 || !(property && property->isSeparator())) 
	{
        const int right = (option.direction == Qt::LeftToRight) ? option.rect.right() : option.rect.left();

        painter->drawLine(right, option.rect.y(), right, option.rect.bottom());
    }

    painter->drawLine(option.rect.x(), option.rect.bottom(),
                      option.rect.right(), option.rect.bottom());
    painter->setPen(savedPen);
}

// ================================================================================================

QSize QPropertyEditorDelegate::sizeHint(const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(opt, index) + QSize(4, 4);
}

// ================================================================================================

bool QPropertyEditorDelegate::isReadOnly() const
{
    return m_readOnly;
}

// ================================================================================================

void QPropertyEditorDelegate::setReadOnly(bool readOnly)
{
    // ### close the editor
    m_readOnly = readOnly;
}

// ================================================================================================

QWidget *QPropertyEditorDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    Q_UNUSED(option);

    QPropertyEditorModel *model = const_cast<QPropertyEditorModel *>(static_cast<const QPropertyEditorModel *>(index.model()));
    const IProperty *property = model->privateData(index);

    if (property == 0)
        return 0;

    QWidget *editor = 0;

	// ### always true
    if (!isReadOnly() && property->hasEditor()) 
	{ 
        if (property->hasReset()) 
		{
            EditorWithReset *editor_w_reset = new EditorWithReset(property, model, parent);
            QWidget *child_editor = property->createEditor(editor_w_reset, editor_w_reset, SIGNAL(sync()));

            editor_w_reset->setChildEditor(child_editor);

            connect(editor_w_reset, SIGNAL(sync()), this, SLOT(sync()));
            connect(editor_w_reset, SIGNAL(resetProperty(const IProperty *, QPropertyEditorModel *)),
                    this, SLOT(resetProperty(const IProperty *, QPropertyEditorModel *)));

            editor = editor_w_reset;

            if (TextPropertyEditor* edit = qobject_cast<TextPropertyEditor*>(child_editor)) 
			{
                // in case of TextPropertyEditor install the filter on it's private QLineEdit
                edit->installEventFilter(const_cast<QPropertyEditorDelegate *>(this));
            } 
			else
                child_editor->installEventFilter(const_cast<QPropertyEditorDelegate *>(this));
        } 
		else 
		{
			EditorWithCommitAndRevert *editor_w_cr = new EditorWithCommitAndRevert(property, model, parent);
			QWidget *child_editor = property->createEditor(editor_w_cr, editor_w_cr, SIGNAL(sync()));

			editor_w_cr->setChildEditor(child_editor);

            connect(editor_w_cr, SIGNAL(sync()), this, SLOT(sync()));

			editor = editor_w_cr;

            if (TextPropertyEditor* edit = qobject_cast<TextPropertyEditor*>(child_editor)) 
			{
                // in case of TextPropertyEditor install the filter on it's private QLineEdit
                edit->installEventFilter(const_cast<QPropertyEditorDelegate *>(this));
            } 
			else
                child_editor->installEventFilter(const_cast<QPropertyEditorDelegate *>(this));
        }
    }

    return editor;
}

// ================================================================================================

void QPropertyEditorDelegate::setEditorData(QWidget *editor,
                             const QModelIndex &index) const
{
    if (EditorWithReset *editor_w_reset = qobject_cast<EditorWithReset*>(editor))
        editor = editor_w_reset->childEditor();

    if (EditorWithCommitAndRevert *editor_w_cr = qobject_cast<EditorWithCommitAndRevert*>(editor))
        editor = editor_w_cr->childEditor();

    const QAbstractItemModel *model = index.model();
    IProperty *property = static_cast<const QPropertyEditorModel*>(model)->privateData(index);

    if (property && property->hasEditor()) 
	{
        property->updateEditorContents(editor);
    }
}

// ================================================================================================

void QPropertyEditorDelegate::setModelData(QWidget *editor,
										   QAbstractItemModel *model,
										   const QModelIndex &index) const
{
    if (EditorWithReset *editor_w_reset = qobject_cast<EditorWithReset*>(editor))
        editor = editor_w_reset->childEditor();

    if (EditorWithCommitAndRevert *editor_w_cr = qobject_cast<EditorWithCommitAndRevert*>(editor))
        editor = editor_w_cr->childEditor();

    if (IProperty *property = static_cast<const QPropertyEditorModel*>(model)->privateData(index)) 
	{
        property->updateValue(editor);

        if (property->propertyName() == QLatin1String("Family") ||
            property->propertyName() == QLatin1String("Point Size") ||
            property->propertyName() == QLatin1String("Bold") ||
            property->propertyName() == QLatin1String("Italic") ||
            property->propertyName() == QLatin1String("Underline") ||
            property->propertyName() == QLatin1String("Strikeout") ||
            property->propertyName() == QLatin1String("Kerning") ||
            property->propertyName() == QLatin1String("Antialiasing")) 
		{
            const QModelIndex parentIndex = index.parent();

            if (IProperty *fontProperty = static_cast<const QPropertyEditorModel*>(model)->privateData(parentIndex)) 
			{
                QFont f = qvariant_cast<QFont>(fontProperty->value());
                if (property->propertyName() == QLatin1String("Family"))
                    f.setFamily(property->toString());
                else if (property->propertyName() == QLatin1String("Point Size"))
                    f.setPointSize(property->value().toInt());
                else if (property->propertyName() == QLatin1String("Bold"))
                    f.setBold(property->value().toBool());
                else if (property->propertyName() == QLatin1String("Italic"))
                    f.setItalic(property->value().toBool());
                else if (property->propertyName() == QLatin1String("Underline"))
                    f.setUnderline(property->value().toBool());
                else if (property->propertyName() == QLatin1String("Strikeout"))
                    f.setStrikeOut(property->value().toBool());
                else if (property->propertyName() == QLatin1String("Kerning"))
                    f.setKerning(property->value().toBool());
                else if (property->propertyName() == QLatin1String("Antialiasing"))
                    f.setStyleStrategy((QFont::StyleStrategy)property->value().toInt());
                fontProperty->setValue(f);
                model->setData(parentIndex, f, Qt::EditRole);
                return;
            }
        }

        model->setData(index, property->value(), Qt::EditRole);
    }
}

// ================================================================================================

void QPropertyEditorDelegate::drawDecoration(QPainter *painter, const QStyleOptionViewItem &option,
                              const QRect &rect, const QPixmap &pixmap) const
{
    QItemDelegate::drawDecoration(painter, option, rect, pixmap);
}

// ================================================================================================

void QPropertyEditorDelegate::sync()
{
    QWidget *w = qobject_cast<QWidget*>(sender());

    if (w == 0)
        return;

    emit commitData(w);
}

// ================================================================================================

void QPropertyEditorDelegate::resetProperty(const IProperty *property, 
											QPropertyEditorModel *model)
{
    emit resetProperty(property->propertyName());
}

// ================================================================================================

void QPropertyEditorDelegate::updateEditorGeometry(QWidget *editor, 
												   const QStyleOptionViewItem &option, 
												   const QModelIndex &index) const
{
    QItemDelegate::updateEditorGeometry(editor, option, index);
    editor->setGeometry(editor->geometry().adjusted(0, 0, -1, -1));
}
