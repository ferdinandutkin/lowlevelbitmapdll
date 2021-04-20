#include "pch.h" 
#include "KMPDLL.h"



int ShowBitMap(HWND hWnd,	
	HBITMAP hBit,			 
	int x, int y)			 
{
	BITMAP BitMap;
	GetObjectW(hBit, sizeof(BITMAP), &BitMap);
	int Height = BitMap.bmHeight;
	int Width = BitMap.bmWidth;
	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, hBit);
	BitBlt(hdc, x, y, Width, Height, hdcMem, 0, 0, SRCCOPY);
	return 0;
}


int ShowBitMapFromFile(HWND hWnd,	 
	const wchar_t* Name,			 
	int x, int y)		 
{
	HBITMAP hBit = (HBITMAP)LoadImageW(NULL, Name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BITMAP BitMap;
	GetObjectW(hBit, sizeof(BITMAP), &BitMap);
	int Height = BitMap.bmHeight;
	int Width = BitMap.bmWidth;
	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, hBit);
	BitBlt(hdc, x, y, Width, Height, hdcMem, 0, 0, SRCCOPY);
	return 0;
}





int ClientToBmp(HWND hWnd,	
	const wchar_t* Name)				
{
	RECT r;
	GetClientRect(hWnd, &r); 
	return ClientRectToBmp(hWnd, Name, r);
}




int ClientRectToBmp(HWND hWnd, const wchar_t* name, RECT r)
{
	HANDLE fh = CreateFileW(name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	BITMAPINFOHEADER bi{
		.biSize = sizeof(BITMAPINFOHEADER) ,
		.biWidth = r.right - r.left,
	    .biHeight = r.bottom - r.top,
        .biPlanes = 1,
		.biBitCount = 32,
		.biSizeImage = (bi.biWidth * bi.biBitCount + 31) / 32 * 4 * bi.biHeight
	};

	BITMAPFILEHEADER bmfHdr{
		.bfType = 0x4D42,
		.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + bi.biSize,
		.bfReserved1 = bmfHdr.bfReserved2 = 0,
		.bfOffBits = sizeof(BITMAPFILEHEADER) + bi.biSize
	};
	 

	HDC hDC = GetDC(hWnd);
	HDC hDCMem = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, bi.biWidth, bi.biHeight);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hDCMem, hBitmap);
	BitBlt(hDCMem, 0, 0, bi.biWidth, bi.biHeight, hDC, r.left, r.top, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hDCMem, oldBitmap);

	HANDLE hDIB = GlobalAlloc(GHND, bi.biSizeImage);
	char* lp = (char*)GlobalLock(hDIB);
	GetDIBits(hDC, hBitmap, 0, bi.biHeight, lp, &bi, DIB_RGB_COLORS);
	DWORD dwWritten = sizeof(BITMAPFILEHEADER);

	WriteFile(fh, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	dwWritten = sizeof(BITMAPINFOHEADER);
	WriteFile(fh, &bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	dwWritten = bi.biSizeImage;
	WriteFile(fh, lp, bi.biSizeImage, &dwWritten, NULL);

	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	DeleteObject(hBitmap);
	lp = NULL;
	CloseHandle(fh);
	ReleaseDC(hWnd, hDCMem);
	ReleaseDC(hWnd, hDC);
	if (dwWritten == 2) return 2;
 
	return 0;
}