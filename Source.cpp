

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "stdafx.h"
#include "SerialPort.h"


#include <sstream>


using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


/// Function header
void thresh_callback(int, void*);

/**
* @function main
*/

int main(int argc, char** argv)
{
	
	/// Load source image
	String imageName("../data/download.png"); // by default
	if (argc > 1)
	{
		imageName = argv[1];
	}
	src = imread(imageName, IMREAD_COLOR);

	if (src.empty())
	{
		cerr << "No image supplied ..." << endl;
		return -1;
	}

	/// Convert image to gray and blur it
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	

	/// Create Window

	CSerialPort port;
	port.Open(4, 115200, CSerialPort::Parity::NoParity, 8, CSerialPort::StopBits::OneStopBit, CSerialPort::FlowControl::XonXoffFlowControl);
	

	port.Flush();

	port.Close();

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);

	waitKey(0);




	return(0);
}
float map(float A, float A1, float A2, float Min, float Max)
{
	long double percentage = (A - A1) / (A1 - A2);
	return (percentage) * (Min - Max) + Min;
}
/**
* @function thresh_callback
*/
void thresh_callback(int, void*)
{
	std::ostringstream vts;
	CSerialPort port;
	char rxBuf[13] = "TTTTTTTTTTTT";
	port.Open(4, 115200, CSerialPort::Parity::NoParity, 8, CSerialPort::StopBits::OneStopBit, CSerialPort::FlowControl::XonXoffFlowControl);

	COMMTIMEOUTS timeouts{};
	port.GetTimeouts(timeouts);

	port.Setup(10000, 10000);
	port.Set0WriteTimeout();
	port.Set0ReadTimeout();

	Mat canny_output;
	Mat contour;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int i, j, con1, con2;
	con1 = (255 / 230);
	con2 = (300 / 219);
	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	String imageName("../data/download.png"); // by default
	src = imread(imageName, IMREAD_COLOR);
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	src = src > 1;
	
	Scalar color(40, 40, 40);

	for (i = 0; i < contours.size(); i++)
	{
		for (j = 0; j < contours[i].size(); j++)
		{
			contours[i][j].x = map(contours[i][j].x,0, src.cols,0,600);
			contours[i][j].y = map(contours[i][j].y, 0, src.rows, 0, 510);



			port.Write("(", 1);

			string xcoor = to_string({ contours[i][j].x });
			const char* x = { xcoor.c_str() };
			port.Write(x, (strlen(x)));
			cout << "VISUAL X : " << x << endl;

			port.Write(",", 1);

			string ycoor = to_string({ contours[i][j].y });
			const char* y = { ycoor.c_str() };
			port.Write(y, (strlen(y)));
			cout << "VISUAL Y : " << y << endl;

			port.Write(")", 1);

			bool pass = true;	
		
			while (pass) {
				port.TransmitChar('w');
				memset(rxBuf, 0, sizeof(rxBuf));
				port.Read(rxBuf, 13);
				cout << "Arduino : " << rxBuf;
				cout << endl;
				
				
				
				if (rxBuf[0] == (char)'z') { pass = false; memset(rxBuf, 0, sizeof(rxBuf)); }
				
			}
			
		}
	}

}







