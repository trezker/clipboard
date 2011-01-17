#include "clipboard.h"
#include <stdio.h>

const char* Get_clipboard_errmsg(void)
{
   return "Error message not set";
}
int Set_clipboard_text(const char * const text, const int size)
{
	printf("Windows version, not implemented yet");
	return 0;
}

const char * Get_clipboard_text()
{
	printf("Windows version, not implemented yet");
    return 0;
}

