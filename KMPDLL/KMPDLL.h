 
#pragma once
#include <wtypes.h>

#ifdef KMPDLL_EXPORTS
#define KMPDLL_API __declspec(dllexport)
#else
#define KMPDLL_API __declspec(dllimport)
#endif


extern "C" KMPDLL_API 
	int ShowBitMap(HWND hWnd,  HBITMAP hBit, int x, int y);

extern "C" KMPDLL_API int ClientToBmp(HWND hWnd,
	const wchar_t* Name);


extern "C" KMPDLL_API
	int ShowBitMapFromFile(HWND hWnd, const wchar_t* Name, int x, int y);


extern "C" KMPDLL_API
	int ClientRectToBmp(HWND hWnd, const wchar_t* name, RECT r);
