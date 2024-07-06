#include <iostream>
#include "Test.h"

#include <opencv2/opencv.hpp>


using namespace std;

int main(int argc, char** argv)
{
	//MakeScreenShot();

	if(argc < 2) // Check if an argument is provided
	{
		std::cerr << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
		return -1;
	}

	cv::String ImageName("Data\\screen.png");
	cv::Mat image;
	try
	{
		image = cv::imread(ImageName);
	}
	catch(const cv::Exception& ex)
	{
		std::cerr << "Error: Could not open or find the image!" << std::endl;
	}

	
	if(image.empty()) // Check if the image was loaded successfully
	{
		std::cerr << "Error: Could not open or find the image!" << std::endl;
		return -1;
	}


	return 0;
}