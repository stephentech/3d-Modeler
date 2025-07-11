
#ifndef _DXMAIN_H_
#define _DXMAIN_H_

#include <windows.h>

void dxInit(HWND);
void dxRender();
void dxCleanup();

void dxLeftClickDown(int, int);
void dxLeftClickUp(int, int);

void dxRightClickDown(int, int);
void dxRightClickUp(int, int);

void dxKeyDown(char);
void dxKeyUp(char);

#endif
