/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech AS. All rights reserved.
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

#include "FlagBox.h"

#include <QtGui/QAbstractItemView>
#include <QtGui/QItemDelegate>

#include <QtCore/qdebug.h>

// using namespace qdesigner_internal;

FlagBox::FlagBox(QWidget *parent)
    : QComboBox(parent)
{
    m_model = new FlagBoxModel(this);
    setModel(m_model);

#ifdef Q_WS_MAC
    setItemDelegate(new QItemDelegate(this));
#endif

    connect(this, SIGNAL(activated(int)), this, SLOT(slotActivated(int)));
}

FlagBox::~FlagBox()
{
}

void FlagBox::slotActivated(int index)
{
    QVariant value = itemData(index, Qt::CheckStateRole);
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    setItemData(index, (state == Qt::Unchecked ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
}
