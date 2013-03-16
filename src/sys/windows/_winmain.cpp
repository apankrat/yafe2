#include <windows.h>

#include <stdio.h>
#include "console.h"

/*
 *	real entry point
 */
int wmain(int argc, wchar_t ** argv);

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prev_instance, 
                       LPWSTR cmd_line, int show_cmd)
{
	extern wchar_t ** __wargv;
	extern int __argc;
	int rc;

//	open_console();
	
	rc = wmain(__argc, __wargv);

//	printf("\nPress Enter to exit...\n");
//	getchar();
}

/*
 *	super-duper crap-o-matic
 */
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
