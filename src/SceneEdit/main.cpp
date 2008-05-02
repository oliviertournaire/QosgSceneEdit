//=======================================================================================
//  Headerfiles
//=======================================================================================

// Qt
#include <QApplication>
#include <QStyleFactory>
#include <QSplashScreen>

// Project
#include "MainWindow.h"
#include "Styles/seStyle.h"

//=======================================================================================

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

//=======================================================================================

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setStyle(new seStyle());

	QSplashScreen splash(QPixmap(":/Images/se_splash.png"));
	splash.show();
	app.processEvents();

	SceneEdit::MainWindow* mainWindow = new SceneEdit::MainWindow();
	mainWindow->show();

	splash.finish(mainWindow);

	return app.exec();
}

//=======================================================================================
