#pragma once

#include <QObject>
#include <Qsci/qsciscintilla.h>

#include "seQtExt.h"

class QRegExp;

class SEQTEXT_EXPORT Shell : public QsciScintilla
{
	Q_OBJECT

public:

	Shell(QWidget *parent = 0L);

public Q_SLOTS:

	void completionListSelected();
	void write(QString s);
	void writeStdOut(QString s);

private:

	void getEndPos(int *line, int *column) const;

	QRegExp *_linesepRegExp;
};
