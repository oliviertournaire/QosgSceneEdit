#include "PythonShell.h"
#include "QDebugStream.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>


PythonShell::PythonShell(QWidget *parent)
: QWidget(parent)
{
	_lineEdit = new QLineEdit();
	_lineEdit->setFrame(false);

	_textEdit = new QTextEdit();
	_textEdit->setFrameStyle(QFrame::NoFrame);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(_textEdit);
	layout->addWidget(_lineEdit);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	_stdoutStream = new QDebugStream(std::cout);
	connect(_stdoutStream, SIGNAL(clientProcessStdout(QString)), _textEdit, SLOT(append(QString)));
}

PythonShell::~PythonShell()
{
	if (_stdoutStream)
	{
		delete _stdoutStream;
		_stdoutStream = 0L;
	}
}
