// Qt
#include <qapplication.h>
#include <QStyleFactory>
#include "MainWindow.h"
#include "Styles/seStyle.h"

// Project
#ifdef Q_WS_WIN
#include "Styles/qtdotnetstyle.h"
#endif

#if defined(Q_WS_WIN)
	#if defined _M_IX86
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_IA64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_X64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#else
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#endif
#endif

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	app.setStyle(new seStyle());

	MainWindow* mainWindow = new MainWindow();
	mainWindow->show();

	return app.exec();
}
