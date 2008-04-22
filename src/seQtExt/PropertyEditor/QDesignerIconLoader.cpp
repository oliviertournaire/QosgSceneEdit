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

#include "QDesignerIconLoader.h"

#include <QtCore/QFile>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>


QIcon createIconSet(const QString &name)
{
    QStringList candidates = QStringList()
        << (QString::fromUtf8(":/SceneEdit/QtExt/images/") + name)
#ifdef Q_WS_MAC
        << (QString::fromUtf8(":/SceneEdit/QtExt/images/mac/") + name)
#else
        << (QString::fromUtf8(":/SceneEdit/QtExt/images/win/") + name)
#endif
        << (QString::fromUtf8(":/SceneEdit/QtExt/images/designer_") + name);

    foreach (QString f, candidates) 
    {
        if (QFile::exists(f))
            return QIcon(f);
    }

    return QIcon();
}

QIcon emptyIcon()
{
    static const QIcon empty_icon(QLatin1String(":/SceneEdit/QtExt/images/emptyicon.png"));
    return  empty_icon;
}
