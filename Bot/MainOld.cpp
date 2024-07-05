#include <iostream>
#include <windows.h>
#include <psapi.h>

#include <gdiplus.h>

struct ScrenData
{
	int ScreenWidth;
	int	ScreenHeigh;
	unsigned char* ImageBuffer;
};

bool SaveTga(ScrenData& SDt, const char* url)
{
	//
   // Теперь нужно записать полученные данные в файл Screen.tga.
   //

	FILE* sFile = 0;        // Дескриптор файла

	// Обьявляем переменные, которые понадобятся нам в дальнейшем:
	unsigned char tgaHeader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6];
	unsigned char tempColors = 0;

	// Открываем файл скриншота
	fopen_s(&sFile, url, "wb");

	// Проверяем, правильно ли произошло открытие
	if(!sFile) {
		return 0;
	}



	// Записываем ширину и высоту:
	header[0] = SDt.ScreenWidth % 256;
	header[1] = SDt.ScreenWidth / 256;
	header[2] = SDt.ScreenHeigh % 256;
	header[3] = SDt.ScreenHeigh / 256;
	header[4] = 24;
	header[5] = 0;

	// Записываем хидеры в начало файла:
	fwrite(tgaHeader, sizeof(tgaHeader), 1, sFile);
	fwrite(header, sizeof(header), 1, sFile);
	// Записываем данные изображения:
	fwrite(SDt.ImageBuffer, SDt.ScreenWidth * SDt.ScreenHeigh * 3, 1, sFile);

	// Закрываем файл
	fclose(sFile);
	// Удаляем ненужные теперь данные

	return 1;

}


int main()
{
	Sleep(5000);

	std::string ClassName("trinityWindow");

	auto Win = FindWindowA(ClassName.c_str(), nullptr);

	auto DC = GetDC(Win);

	HDC hdcMem = CreateCompatibleDC (DC);

	RECT  rect;
	
	GetWindowRect(Win, &rect);

	ScrenData SDt;
	SDt.ScreenHeigh = rect.bottom - rect.top;
	SDt.ScreenWidth = rect.right - rect.left;

	BITMAPINFO BMI;
	BMI.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BMI.bmiHeader.biWidth = SDt.ScreenWidth;
	BMI.bmiHeader.biHeight = SDt.ScreenHeigh; // Отрицательное значение высоты, чтобы изображение не было перевёрнутым
	BMI.bmiHeader.biSizeImage = SDt.ScreenWidth * SDt.ScreenHeigh * 3; // Ширина * Высота * Количество_цветов_на_пиксель
	BMI.bmiHeader.biCompression = BI_RGB;
	BMI.bmiHeader.biBitCount = 24;
	BMI.bmiHeader.biPlanes = 1;


	HBITMAP hBitmap = CreateDIBSection(DC,&BMI,DIB_RGB_COLORS,(void**)&SDt.ImageBuffer,0,0);
	SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, SDt.ScreenWidth, SDt.ScreenHeigh, DC , 0, 0, SRCCOPY);

	const char* name = "scren.tga";

	SaveTga(SDt, name);


	DWORD pid;
	GetWindowThreadProcessId(Win, &pid);
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
		FALSE, pid);
	TCHAR szPath[MAX_PATH];
	if(GetProcessImageFileName(hProc, szPath, sizeof(szPath)))
	{
		std::cout << szPath << std::endl;
	}
	CloseHandle(hProc);
}

#pragma comment(lib,"user32")
