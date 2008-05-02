//==============================================================================
//  Headerfiles
//==============================================================================

// Project
#include "PythonShell.h"
#include "QDebugStream.h"

// Qt
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextStream>

// OpenSceneGraph
#include <osg/Notify>

// C-Lib
#include <cstdio>

// Boost
#include <boost/python.hpp>

//==============================================================================

static void PythonShellLogInfo(const std::string& str)
{
	std::cout << str;
}

//==============================================================================

static void PythonShellLogError(const std::string& str)
{
	std::cerr << str;
}

//==============================================================================

BOOST_PYTHON_MODULE(PythonShell)
{
	using namespace boost::python;

	def("PythonShellLogInfo", PythonShellLogInfo);
	def("PythonShellLogError", PythonShellLogError);
}

//==============================================================================

PythonShell::PythonShell(QWidget *parent)
: QWidget(parent)
, _stdoutColor(220,220,220)
, _stderrColor(255,64,64)
{
	_lineEdit = new QLineEdit();
	_lineEdit->setFrame(false);
	_lineEdit->setFocusPolicy(Qt::NoFocus);

	_textEdit = new QTextEdit();
	_textEdit->setFrameStyle(QFrame::NoFrame);
    _textEdit->setReadOnly(true);
	_textEdit->setFocusPolicy(Qt::NoFocus);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(_textEdit);
	layout->addWidget(_lineEdit);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	setFocusPolicy(Qt::StrongFocus);
	setFocusProxy(_lineEdit);

	// Redirect std::cout to the appendStdout method
	_stdoutStream = new QDebugStream(std::cout);
	connect(_stdoutStream, SIGNAL(clientProcessStdout(QString)), SLOT(appendStdout(QString)));

	// Redirect std::cerr to the appendStderr method
	_stderrStream = new QDebugStream(std::cerr);
	connect(_stderrStream, SIGNAL(clientProcessStdout(QString)), SLOT(appendStderr(QString)));

	connect(_lineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));

	// Initialize python and redirect stdout and stderr.
    Py_Initialize();

	initPythonShell();

	PyRun_SimpleString("from PythonShell import *");
	PyRun_SimpleString("class StdoutCatcher:\n"
		                "\tdef __init__(self):\n"
						"\t\tself.data = \"\"\n"
						"\tdef write(self, str):\n"
						"\t\tself.data = self.data + str\n"
						"\t\tif self.data[-1] == \"\\n\":\n"
						"\t\t\tPythonShellLogInfo(self.data)\n"
						"\t\t\tself.data = \"\"\n");
	PyRun_SimpleString("class StderrCatcher:\n"
		                "\tdef __init__(self):\n"
						"\t\tself.data = \"\"\n"
						"\tdef write(self, str):\n"
						"\t\tself.data = self.data + str\n"
						"\t\tif self.data[-1] == \"\\n\":\n"
						"\t\t\tPythonShellLogError(self.data)\n"
						"\t\t\tself.data = \"\"\n");
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.stdout = StdoutCatcher()");
	PyRun_SimpleString("sys.stderr = StderrCatcher()");
}

//==============================================================================

PythonShell::~PythonShell()
{
	if (_stdoutStream)
	{
		delete _stdoutStream;
		_stdoutStream = 0L;
	}

	if (_stderrStream)
	{
		delete _stderrStream;
		_stderrStream = 0L;
	}

	//Py_Finalize();
}

//==============================================================================

void PythonShell::executeCommand()
{
#if 0
	boost::python::api::object main_module = boost::python::import("__main__");
	boost::python::api::object main_namespace(main_module.attr("__dict__"));
	boost::python::str test("print \"Hallo Welt!\"");
	boost::python::api::object result = boost::python::eval(test, main_namespace);
	//std::string value = boost::python::extract<char*>(main_namespace["result"]);

	try
	{
		//int value = boost::python::extract<int>(main_namespace["result"]);
	}
	catch (...)
	{
		PyErr_Print();
		PyErr_Clear();
	}
#endif

	appendStdout(_lineEdit->text());
	PyRun_SimpleString(_lineEdit->text().toLatin1());
	_lineEdit->clear();
}

//==============================================================================

void PythonShell::appendStdout(QString str)
{
	_textEdit->setTextColor(_stdoutColor);
	_textEdit->append(str);
}

//==============================================================================

void PythonShell::appendStderr(QString str)
{
	_textEdit->setTextColor(_stderrColor);
	_textEdit->append(str);
}

//==============================================================================
