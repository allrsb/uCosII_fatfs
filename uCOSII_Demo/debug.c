
//#include <iostream>
#include <windows.h>
#include "debug.h"



char _trace(TCHAR *format, ...)
{
	TCHAR buffer[1000];

	va_list argptr;

	va_start(argptr, format);
	wvsprintf(buffer, format, argptr);
	va_end(argptr);

	OutputDebugString(buffer);

	return 0;
}
