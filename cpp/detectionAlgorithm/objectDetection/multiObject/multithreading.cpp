#include <string>
#include <iostream>
#include <thread>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat imgLines;
Mat imgOriginal;

int iLowH;
int iHighH;
int iLowS; 
int iHighS;
int iLowV;
int iHighV;
int iLastX; 
int iLastY;

// The function we want to execute on the new thread.
void task1(string msg)
{
	cout << "task1 says: " << msg << "\n" << endl;
}

Mat image(Mat &imgOriginal, Mat &imgLines){

	Mat imgHSV;

	//Convert the captured frame from BGR to HSV
	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); 	

	Mat imgThresholded;
	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

	//Calculate the moments of the thresholded image
	Moments oMoments = moments(imgThresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	// if the area <= 10000, I consider that the there are no 
	// object in the image and it's because of the noise, the area is not zero 
	if (dArea > 10000)
	{
		//calculate the position of the ball
		int posX = dM10 / dArea;
		int posY = dM01 / dArea;        

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		{
			//Draw a red line from the previous point to the current point
			line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
		}

		iLastX = posX;
		iLastY = posY;
	}

	return imgThresholded;
}

int main(int argc, char** argv)
{

	VideoCapture cap(0); //capture the video from webcam

	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	iLowH = 0;
	iHighH = 10;

	iLowS = 100; 
	iHighS = 255;

	iLowV = 100;
	iHighV = 255;

	iLastX = -1; 
	iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp); 

	//Create a black image with the size as the camera output
	imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 

	while (true)
	{

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			 cout << "Cannot read a frame from video stream" << endl;
			 break;
		}

		Mat imgThresholded;

		imgThresholded = image(imgOriginal, imgLines);

		imshow("Thresholded Image", imgThresholded); //show the thresholded image

		imgOriginal = imgOriginal + imgLines;
		imshow("Original", imgOriginal); //show the original image

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) 
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}

	}

	// Constructs the new thread and runs it. Does not block execution.
	thread t1(task1, "Hello");
	thread t2(task1, "Hej Hej");
	thread t3(task1, "Hej da");

	// Makes the main thread wait for the new thread to finish execution, 
	// 	therefore blocks its own execution.
	t1.join();
	t2.join();
	t3.join();
}


