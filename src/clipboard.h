#ifndef swag_clipboard_h
#define swag_clipboard_h

char * Get_clipboard_errmsg(void);
int Set_clipboard_text(const char * const text, const int size);
const char * Get_clipboard_text(void);

#endif

