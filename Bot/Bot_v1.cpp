
#include <iostream>
#include <windows.h>
#include "Test.h"

int leftclick(DWORD dwx, DWORD dwy)
{
	//Sleep(2000);

	auto x = 1718; //GetSystemMetrics(SM_CXSCREEN);
	auto y = 878; //GetSystemMetrics(SM_CYSCREEN);

	INPUT input;
	::ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_MOUSE;
	input.mi.dwExtraInfo = 0;
	input.mi.dwFlags =  MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = (65536/x)*dwx;
	input.mi.dy = (65536/y)*dwy;
	SendInput(1, &input, sizeof(input));

	Sleep(500);

	::ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_MOUSE;
	input.mi.dwExtraInfo = 0;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = 0;
	input.mi.dy = 0;
	SendInput(1, &input, sizeof(input));

	Sleep(200);

	::ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_MOUSE;
	input.mi.dwExtraInfo = 0;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = 0;
	input.mi.dy = 0;
	SendInput(1, &input, sizeof(input));
	return 0;
}

void DownKey(char Key)
{
	INPUT input;
	::ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = Key;
	SendInput(1, &input, sizeof(input));
}

void UpKey(char Key)
{
	INPUT input;
	::ZeroMemory(&input, sizeof(INPUT));

	input.type = INPUT_KEYBOARD;
	input.ki.wVk = Key;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(input));
}

void PressKey(char Key)
{
	Sleep(100);
	DownKey(Key);
	Sleep(100);
	UpKey(Key);
}

void UnDock()
{
	std::cout << "UnDock" << std::endl;
	leftclick(906, 285);
	Sleep(25000);
}

void MoveToMission()
{
	std::cout << "MoveToMission" << std::endl;

	leftclick(232, 436);
	Sleep(1000*40);
}

void EndMission()
{
	std::cout << "End mission" << std::endl;
	Sleep(1000*20);
	leftclick(215, 436);
	Sleep(1000*5);
	leftclick(283, 643);
	Sleep(1000*5);
	leftclick(501, 646);
	Sleep(1000*5);
	leftclick(382, 645);
	Sleep(500);
	leftclick(959, 99);
	Sleep(500);
}

void StartTargiting()
{
	DownKey(VK_CONTROL);
}

void StopTargiting()
{
	UpKey(VK_CONTROL);
}

void MoveToFistTarget()
{
	DownKey('Q');
	Sleep(100);
	leftclick(747, 261);
	Sleep(100);
	UpKey('Q');
}

void MoveToOrbit()
{
	DownKey('W');
	Sleep(100);
	leftclick(747, 261);
	Sleep(100);
	UpKey('W');
}

void LaunchDrones()
{
	DownKey(VK_SHIFT);
	DownKey('F');
	Sleep(100);
	UpKey(VK_SHIFT);
	UpKey('F');
}

void CollectDrones()
{
	DownKey(VK_SHIFT);
	DownKey('R');
	Sleep(100);
	UpKey(VK_SHIFT);
	UpKey('R');
}

void Mining()
{
	std::cout << "Start Mining" << std::endl;

	MoveToFistTarget();
	MoveToOrbit();
	//PressKey(VK_F3);

	LaunchDrones();

	Sleep(15000);

	for(int i = 0; i < 2; i++)
	{
		std::cout << "Next asteroid" << std::endl;

		StartTargiting();
		leftclick(747, 261);
		//leftclick(1588, 307);
		StopTargiting();

		//MoveToFistTarget();


		Sleep(3000);
		//PressKey('F');

		//PressKey(VK_F3);
		PressKey(VK_F1);
		PressKey(VK_F2);

		Sleep(1000 * 60 * 4);
		Sleep(4000);

		MoveToOrbit();
		Sleep(3000);
	}

	CollectDrones();
}

void RunBot_v1()
{
	ActiveWindows();
	Sleep(1000);

	//EndMission();
	//return;

	for(int i = 0; i < 10000000000; i++)
	{
		UnDock();
		MoveToMission();
		Mining();

		MoveToMission();
		EndMission();
	}


	//MinimaizeWindows();
}

void RunBot_v2()
{
	Sleep(8000);

	leftclick(100, 100);
	//leftclick(1773, 392);
}