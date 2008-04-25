#include "Shell.h"
#include "QDebugStream.h"

#include <QRegExp>
#include <Qsci/qscilexerpython.h>

Shell::Shell(QWidget *parent)
: QsciScintilla(parent)
, _linesepRegExp(new QRegExp("\r\n|\n|\r"))
{
	setUtf8(true);

	setWhatsThis("<b>The Shell Window</b>");

	connect(this, SIGNAL(userListActivated(int,const QString)), this, SLOT(complitionListSelected()));
	//connect(this, SIGNAL(clientProcessStdout), this, SLOT(writeStdOut));
	//connect(this, SIGNAL(clientOutput), this, SLOT(write));

	QDebugStream *debugStream = new QDebugStream(std::cout);
	bool retval = connect(debugStream, SIGNAL(clientProcessStdout(QString)), this, SLOT(writeStdout(QString)));
		

	this->setLexer(new QsciLexerPython(this));
}

void Shell::completionListSelected()
{
}

void Shell::write(QString s)
{
	int line, col, prline, prcol;

	this->getEndPos(&line, &col);
	this->setCursorPosition(line, col);
	this->insert(s);
	this->getCursorPosition(&prline, &prcol);
	this->ensureCursorVisible();
	this->ensureLineVisible(line);
}

void Shell::writeStdOut(QString s)
{
	write(trUtf8("StdOut: %1").arg(s));	
}

void Shell::getEndPos(int *line, int *column) const
{
	*line = this->lines() - 1;
	*column = this->lineLength(*line);
}
