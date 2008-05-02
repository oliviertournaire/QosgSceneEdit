#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H

//==============================================================================
//  Headerfiles
//==============================================================================

// Project
#include "seQtExt.h"

// Qt
#include <QObject>

// C++-Lib
#include <iostream>
#include <streambuf>
#include <string>

//==============================================================================
//  Class Declaration
//==============================================================================

class SEQTEXT_EXPORT QDebugStream : public QObject,
	                                public std::basic_streambuf<char>
{
	Q_OBJECT

public:

	           QDebugStream(std::ostream &stream);
	virtual    ~QDebugStream();


Q_SIGNALS:

	void clientProcessStdout(QString s);

protected:

	virtual int_type		overflow(int_type v);
	virtual std::streamsize xsputn(const char *p, std::streamsize n);


private:

	std::ostream &m_stream;
	std::streambuf *m_old_buf;
	std::string m_string;
};

//==============================================================================

#endif // QDEBUGSTREAM_H
