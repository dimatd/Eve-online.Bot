//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <gdiplus.h>

using namespace std;

#pragma comment(lib, "GdiPlus.lib") /* ��� ���������������� lib-���� */
using namespace Gdiplus; /* ��� ������, �� ��� �� � ���� ��������� ������ Gdiplus:: */

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };


int main()
{
	Sleep(2000);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC scrdc, memdc;
	HBITMAP membit;
	// �������� HDC �������� �����
	// �������� HWND ��� �������� ����� ������ ����� ����.
	scrdc = GetDC(0);
	// ���������� ���������� ������
	int Height, Width;
	Height = GetSystemMetrics(SM_CYSCREEN);
	Width = GetSystemMetrics(SM_CXSCREEN);
	// ������� ����� DC, ���������� �������������� � ������ �������� � �����.
	memdc = CreateCompatibleDC(scrdc);
	membit = CreateCompatibleBitmap(scrdc, Width, Height);
	SelectObject(memdc, membit);
	// ���������... �����!
	BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	bitmap.Save(L"screen.png", &png);

	DeleteObject(hBitmap);

	//GdiplusShutdown(gdiplusToken);
	return 0;
}