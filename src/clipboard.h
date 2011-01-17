#ifndef swag_clipboard_h
#define swag_clipboard_h

#ifdef __cplusplus
extern "C" {
#endif

const char * Get_clipboard_errmsg(void);
int Set_clipboard_text(const char * const text, const int size);
const char * Get_clipboard_text(void);

#ifdef __cplusplus
}
#endif

#endif

