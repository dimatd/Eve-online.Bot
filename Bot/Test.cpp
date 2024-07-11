#include "Test.h"

#include <windows.h>
#include <iostream>

#include <gdiplus.h>
#include <dwmapi.h>

void ReturnDrons()
{
	std::cout << "Sending 'SHIFT-R'\r\n";
	INPUT inputs[2] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_SHIFT;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = 'R';

	//inputs[2].type = INPUT_KEYBOARD;
	//inputs[2].ki.wVk = 'R';
	//inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	//inputs[3].type = INPUT_KEYBOARD;
	//inputs[3].ki.wVk = VK_SHIFT;
	//inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if(uSent != ARRAYSIZE(inputs))
	{
		std::cout << "SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError());
	}


	Sleep(100);

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = 'R';
	inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_SHIFT;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if(uSent != ARRAYSIZE(inputs))
	{
		std::cout << "SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError());
	}
}

using namespace Gdiplus; /* как хочешь, но мне не в кайф постоянно писать Gdiplus:: */

#pragma comment(lib, "GdiPlus.lib") /* наш многострадальный lib-файл */
#pragma comment(lib, "Dwmapi.lib")


void MakeScreenShot(RECT& wrect)
{
	//Sleep(2000);

	static const GUID png =
	{ 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC scrdc, memdc;
	HBITMAP membit;
	// Получаем HDC рабочего стола
	// Параметр HWND для рабочего стола всегда равен нулю.
	scrdc = GetDC(0);
	// Определяем разрешение экрана
	int Height, Width;

	std::string ClassName("trinityWindow");
	auto EveWin = FindWindowA(ClassName.c_str(), nullptr);

	//RECT wrect;
	DwmGetWindowAttribute(EveWin, DWMWA_EXTENDED_FRAME_BOUNDS, &wrect, sizeof(wrect));

	Width = wrect.right - wrect.left;
	Height = wrect.bottom - wrect.top;

	auto CurrentWindows = GetForegroundWindow();

	Sleep(500);

	if(IsIconic(EveWin))
	{
		ShowWindow(EveWin, SW_SHOWNORMAL);
		Sleep(500);
	}

	SetForegroundWindow(EveWin);
	Sleep(300);

	//ReturnDrons();

	memdc = CreateCompatibleDC(scrdc);
	membit = CreateCompatibleBitmap(scrdc, Width, Height);
	SelectObject(memdc, membit);
	// Улыбаемся... Снято!
	BitBlt(memdc, 0, 0, Width, Height, scrdc, wrect.left, wrect.top, SRCCOPY);
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	bitmap.Save(L"Data/screen.png", &png);
	//bitmap.

	DeleteObject(hBitmap);

	SetForegroundWindow(CurrentWindows);
}

void ActiveWindows()
{
	std::string ClassName("trinityWindow");
	auto EveWin = FindWindowA(ClassName.c_str(), nullptr);

	//Sleep(200);

	if(IsIconic(EveWin))
	{
		ShowWindow(EveWin, SW_SHOWNORMAL);
		Sleep(500);
	}

	SetForegroundWindow(EveWin);
	Sleep(300);
}

void MinimaizeWindows()
{
	std::string ClassName("trinityWindow");
	auto EveWin = FindWindowA(ClassName.c_str(), nullptr);
	ShowWindow(EveWin, SW_MINIMIZE);
}
