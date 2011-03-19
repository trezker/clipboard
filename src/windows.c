#include "clipboard.h"
#include <stdio.h>
#include <allegro5/allegro_windows.h>

int Set_clipboard_text(const char * const text, const int size)
{
	HGLOBAL clipBuffer = 0;
	char* buffer = 0;
	HWND allegro_window = al_get_win_window_handle();

	if (!cstr)
	{
		return 1;
	}
	if (OpenClipboard(allegro_window) == 0)
	{
		return 1;
	}

	EmptyClipboard();

	clipBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT , strlen(cstr) + 1);
	buffer = (char*)GlobalLock(clipBuffer);
	strcpy(buffer, cstr);
	GlobalUnlock(clipBuffer);

	if (SetClipboardData(CF_TEXT , clipBuffer) == (void*)0)
	{
		CloseClipboard();
		return 1;
	}
	CloseClipboard();
	return 0;
}

const char * Get_clipboard_text()
{
	char* clipstr = 0;
	char* newstr = 0;
	HWND allegro_window = al_get_win_window_handle();
	HANDLE cliphandle = 0;

	if (IsClipboardFormatAvailable(CF_TEXT) == 0)
	{
		return (char*)0;
	}

	if (OpenClipboard(allegro_window) == 0)
	{
		return (char*)0;
	}
	cliphandle = GetClipboardData(CF_TEXT);
	if (cliphandle == (void*)0) {
		CloseClipboard();
		return (char*)0;
	}
	clipstr = (char*)GlobalLock(cliphandle);
	newstr = strdup(clipstr);
	GlobalUnlock(cliphandle);
	CloseClipboard();
	return newstr;
}

int Set_clipboard_bitmap(ALLEGRO_BITMAP* bmp , const char* image_extension) {
	HWND allegro_window = al_get_win_window_handle();
	HBITMAP hbitmap = 0;

	if (!bmp)
	{
		return 1;
	}
	hbitmap = convert_bitmap_to_hbitmap(bmp);
	if (!bmp)
	{
		return 1;
	}

	if (OpenClipboard(allegro_window) == 0)
	{
		return 1;
	}

	if (SetClipboardData(CF_BITMAP , hbitmap) == (void*)0)
	{
		CloseClipboard();
		return 1;
	}
	CloseClipboard();
	return 0;
}

ALLEGRO_BITMAP* Get_clipboard_bitmap(const char* image_extension) {
	HBITMAP clipbmp = 0;
	ALLEGRO_BITMAP* bmp = 0;
	HWND allegro_window = al_get_win_window_handle();
	HANDLE cliphandle = 0;

	if (IsClipboardFormatAvailable(CF_BITMAP) == 0)
	{
		return (ALLEGRO_BITMAP*)0;
	}

	if (OpenClipboard(allegro_window) == 0)
	{
		return (ALLEGRO_BITMAP*)0;
	}
	cliphandle = GetClipboardData(CF_BITMAP);
	if (cliphandle == (void*)0)
	{
		CloseClipboard();
		return (ALLEGRO_BITMAP*)0;
	}
	clipbmp = (HBITMAP)cliphandle;
	bmp = convert_hbitmap_to_bitmap(clipbmp);
	CloseClipboard();
	return bmp;
}
