#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>

static HANDLE console;

bool open_console()
{
	DWORD mode;

	assert(! console);

	if (! AllocConsole())
		return false;

	/*
	 *	this enables selecting parts of console with a mouse
	 *	and then copying to the clipboard with Enter
	 */
	console = GetStdHandle(STD_INPUT_HANDLE);
	if (GetConsoleMode(console, &mode))
		SetConsoleMode(console, mode | ENABLE_QUICK_EDIT_MODE);

	/*
	 *	self-explanatory
	 */
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleOutputCP(CP_UTF8);

	/*
	 *	this attaches keypresses in a console window to getchar() and alike
	 *	it also enables ::printf'ing to the console
	 */
#pragma warning (disable: 4311)
	*stdin  = *_fdopen(_open_osfhandle((long) GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");
	*stdout = *_fdopen(_open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
#pragma warning (default: 4311)

	setvbuf( stdin,  NULL, _IONBF, 0 );
	setvbuf( stdout, NULL, _IONBF, 0 );

	console = console;
	return true;
}

void close_console()
{
	if (! console)
		return;

	FreeConsole();
	console = NULL;
}
