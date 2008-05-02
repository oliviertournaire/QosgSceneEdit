#pragma once

#include <QFrame>
#include <QLineEdit>

#include "seQtExt.h"

class QTextEdit;
class QDebugStream;

class SEQTEXT_EXPORT PythonShell : public QFrame
{
	Q_OBJECT

public:

	PythonShell(QWidget *parent = 0L);
	virtual ~PythonShell();

protected slots:

	void executeCommand();
	void appendStdout(QString str);
	void appendStderr(QString str);

private:

	QLineEdit *_lineEdit;
	QTextEdit *_textEdit;
	QDebugStream *_stdoutStream;
	QDebugStream *_stderrStream;
	QColor _stdoutColor;
	QColor _stderrColor;
	QColor _promptColor;
	QString _prompt;
};

class MyLineEdit : public QLineEdit
{
	Q_OBJECT

public:

	MyLineEdit(QWidget *parent = 0L);
	virtual ~MyLineEdit();

protected:

	void keyPressEvent(QKeyEvent *event);

private:

	QStringList _history;
	int _historyIndex;
};
