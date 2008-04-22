#include "QDebugStream.h"

#include <QtGui/QTextEdit>
#include <streambuf>


QDebugStream::QDebugStream(std::ostream &stream) 
: m_stream(stream)
{
	m_old_buf = stream.rdbuf();
	stream.rdbuf(this);
}

QDebugStream::~QDebugStream()
{
	// output anything that is left
	if (!m_string.empty())
	{
		emit clientProcessStdout(m_string.c_str());
	}

	m_stream.rdbuf(m_old_buf);
}

int QDebugStream::overflow(int_type v)
{
	if (v == '\n')
	{
		emit clientProcessStdout(m_string.c_str());
		m_string.erase(m_string.begin(), m_string.end());
	}
	else
		m_string += v;

	return v;
}

std::streamsize QDebugStream::xsputn(const char *p, std::streamsize n)
{
	m_string.append(p, p + n);

	int pos = 0;
	while (pos != std::string::npos)
	{
		pos = m_string.find('\n');
		if (pos != std::string::npos)
		{
			std::string tmp(m_string.begin(), m_string.begin() + pos);
			emit clientProcessStdout(m_string.c_str());
			m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
		}
	}

	return n;
}
