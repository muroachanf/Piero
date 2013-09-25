/* File custom.h
 * (custom control interface)
 */
#ifndef CUSTOM_H
#define CUSTOM_H

#include <tchar.h>
#include <windows.h>


/* Window class */
#define CUSTOM_WC   _T("CustomControl")

/* Register/unregister the window class */
void CustomRegister(void);
void CustomUnregister(void);
const int line_width = 50;
const int line_height = 20;
const int line_cell_xcount = 1;
const int line_cell_ycount = 50;

#endif  /* CUSTOM_H */

