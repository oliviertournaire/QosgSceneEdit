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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QtGui/QTextEdit>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include "SharedGlobal.h"

class QDesignerFormWindowInterface;

//namespace qdesigner_internal {

class StyleSheetEditor : public QTextEdit
{
    Q_OBJECT

public:
    StyleSheetEditor(QWidget *parent = 0);
};

class StyleSheetEditorDialog : public QDialog
{
    Q_OBJECT
public:
    StyleSheetEditorDialog(QWidget *parent, QWidget *widget);
    StyleSheetEditor *editor() const;

    static bool isStyleSheetValid(const QString &styleSheet);

private slots:
    void applyStyleSheet();
    void validateStyleSheet();

private:
    StyleSheetEditor *m_editor;
    QDesignerFormWindowInterface *m_fw;
    QWidget *m_widget;
    QLabel *validityLabel;
};

//} // namespace qdesigner_internal

#endif // STYLESHEETEDITOR_H
