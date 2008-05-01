#include "PythonShell.h"
#include "QDebugStream.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextStream>

#include <cstdio>

#include <boost/python.hpp>
//#include <Python.h>

PythonShell *__globalShell = 0;

static PyObject* redirector_init(PyObject*, PyObject*)
{
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject* redirector_write(PyObject*, PyObject *args)
{
	char *output;
	PyObject *self;

	if (!PyArg_ParseTuple(args, "Os", &self, &output))
		return 0;
    
    __globalShell->appendOutput(output);
//     printf("%s", output);
//     fflush(stdout);

	//if (_globalShell)
	//{
	//	_globalShell->appendOutput(output);
	//}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef ModuleMethods[] = { {NULL,NULL,0,NULL} };
static PyMethodDef redirectorMethods[] =
{
    {"__init__", redirector_init, METH_VARARGS, "initialize the stdout/err redirector"},
    {"write", redirector_write, METH_VARARGS, "implement the write method to redirect stdout/err"},
    {NULL,NULL,0,NULL}
};

void initredirector()
{
    PyMethodDef *def;

    /* create a new module and class */
    PyObject *module = Py_InitModule("redirector", ModuleMethods);
    PyObject *moduleDict = PyModule_GetDict(module);
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("redirector");
    PyObject *fooClass = PyClass_New(NULL, classDict, className);
    PyDict_SetItemString(moduleDict, "redirector", fooClass);
    Py_DECREF(classDict);
    Py_DECREF(className);
    Py_DECREF(fooClass);

    /* add methods to class */
    for (def = redirectorMethods; def->ml_name != NULL; def++)
	{
        PyObject *func = PyCFunction_New(def, NULL);
        PyObject *method = PyMethod_New(func, NULL, fooClass);
        PyDict_SetItemString(classDict, def->ml_name, method);
        Py_DECREF(func);
        Py_DECREF(method);
    }
}


PythonShell::PythonShell(QWidget *parent)
: QWidget(parent)
{
    __globalShell = this;
    
	_lineEdit = new QLineEdit();
	_lineEdit->setFrame(false);

	_textEdit = new QTextEdit();
	_textEdit->setFrameStyle(QFrame::NoFrame);
    _textEdit->setReadOnly(true);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(_textEdit);
	layout->addWidget(_lineEdit);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
    
//     _stdoutInterceptor = new Interceptor(this);
//     _stdoutInterceptor->initialize(1);
//     connect(_stdoutInterceptor, SIGNAL(received(QTextStream*)), SLOT(displayPrompt()));
//     
//     _stderrInterceptor = new Interceptor(this);
//     _stderrInterceptor->initialize(2);
//     connect(_stderrInterceptor, SIGNAL(received(QTextStream*)), SLOT(displayPrompt()));

	_stdoutStream = new QDebugStream(std::cout);
	connect(_stdoutStream, SIGNAL(clientProcessStdout(QString)), _textEdit, SLOT(append(QString)));
	connect(_lineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));

    Py_Initialize();    
	initredirector();

	int result = PyRun_SimpleString("import sys\n"
               "import redirector\n"
               "sys.stdout = redirector.redirector()\n"
               "sys.stderr = sys.stdout\n");
}

PythonShell::~PythonShell()
{
	if (_stdoutStream)
	{
		delete _stdoutStream;
		_stdoutStream = 0L;
	}

	//Py_Finalize();
}

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

	PyRun_SimpleString(_lineEdit->text().toLatin1());
	_lineEdit->clear();
}

void PythonShell::appendOutput(QString output)
{
/*    _textEdit->append(output.replace("\n", ""));*/
    _textEdit->append(output);
}

void PythonShell::displayPrompt()
{
/*    if (_stdoutInterceptor)
    {
        QString line;
        QTextStream *s = _stdoutInterceptor->textIStream();        
        
        line = s->readAll();
        _textEdit->setTextColor(Qt::white);
        _textEdit->append(line);
    }
    
    if (_stderrInterceptor)
    {
        QString line;
        QTextStream *s = _stderrInterceptor->textIStream();        
        
        line = s->readAll();
        _textEdit->setTextColor(Qt::red);
        _textEdit->append(line);
    }*/
}
