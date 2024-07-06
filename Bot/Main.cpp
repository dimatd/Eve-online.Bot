#include <iostream>
#include "Test.h"

#include <windows.h>

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

//#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <dwmapi.h>
using namespace std;
using namespace cv;

Mat hwnd2mat() 
{
	std::string ClassName("trinityWindow");
	auto EveWin = FindWindowA(ClassName.c_str(), nullptr);


	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(0);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	//GetClientRect(hwnd, &windowsize);

	DwmGetWindowAttribute(EveWin, DWMWA_EXTENDED_FRAME_BOUNDS, &windowsize, sizeof(windowsize));

	srcheight = windowsize.bottom - windowsize.top;
	srcwidth = windowsize.right - windowsize.left;
	height = windowsize.bottom - windowsize.top;  //change this to whatever size you want to resize to
	width = windowsize.right - windowsize.left;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, windowsize.left, windowsize.top, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	// avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(0, hwindowDC);

	return src;
}


int main(int argc, char** argv)
{
	ActiveWindows();

	//int Height, Width;

	//Width = wrect.right - wrect.left;
	//Height = wrect.bottom - wrect.top;

	//cv::String ImageName("Data\\screen.png");
	//cv::Mat image = cv::imread(ImageName);
	//cv::Mat image2 = image.clone();

	std::string ClassName("trinityWindow");
	auto EveWin = FindWindowA(ClassName.c_str(), nullptr);

	DWORD dwProcID = 0;
	GetWindowThreadProcessId(EveWin, &dwProcID);


	EveWin = FindWindowEx(NULL, EveWin, NULL, NULL);

	cv::Mat image = hwnd2mat();
	cv::Mat image2 = image.clone();
	
	if(image.empty()) // Check if the image was loaded successfully
	{
		std::cerr << "Error: Could not open or find the image!" << std::endl;
		return -1;
	}

	cv::cvtColor(image, image, cv::COLOR_BGRA2GRAY);



	cv::inRange(image, Scalar(0, 0, 0), Scalar(30, 0, 0), image);
	//cv::threshold(image, image, 128, 255, ThresholdTypes::THRESH_BINARY);

	////Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
	std::vector<std::vector<cv::Point> > contours;
	cv::Mat contourOutput = image.clone();
	cv::findContours(contourOutput, contours, RETR_LIST, CHAIN_APPROX_NONE);

	//Draw the contours
	cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Scalar colors[3];
	colors[0] = cv::Scalar(255, 0, 0);
	colors[1] = cv::Scalar(0, 255, 0);
	colors[2] = cv::Scalar(0, 0, 255);
	for(size_t idx = 0; idx < contours.size(); idx++)
	{
		auto contour = contours[idx];
		/*if(contour.size() != 4)
		{
			continue;
		}
		*/

		//cv::drawContours(contourImage, contours, idx, colors[idx % 3]);

		auto rect = cv::minAreaRect(contour); // �������� ������� �������������

		auto Rect2 = rect.boundingRect();


		if(10000 < Rect2.width * Rect2.height)
		{
			rectangle(image2, Rect2, colors[idx % 3], 2);
		}



		//Mat boxPts;
		//cv::boxPoints(rect, boxPts);
		
		//cv::rectangle(image, rect., p2,
		//	Scalar(255, 0, 0),
		//	thickness, LINE_8);
		//	//box = np::int0(box) # ���������� ���������
		//cv::drawContours(contourImage, boxPts, 0, colors[idx % 3]); // ������ �������������
	}

	//cv::imshow("Input Image", image);
	//cv::moveWindow("Input Image", 0, 0);

	cv::imshow("Contours", image2);
	cv::moveWindow("Contours", 0, 0);
	//cv::waitKey(0);

	MinimaizeWindows();

	//cv::imshow("Display window", image);
	auto k = cv::waitKey(0);

	return 0;
}