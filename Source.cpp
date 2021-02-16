
/**
* @function findContours_Demo.cpp
* @brief Demo code to find contours in an image
* @author OpenCV team
*/

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "stdafx.h"
#include "SerialPort.h"


#include <sstream>


using namespace cv;
using namespace std;
//char* portName = "4";
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

		//CSerialPort port;
		//port.Open(4, 9600, CSerialPort::Parity::NoParity, 8, CSerialPort::StopBits::OneStopBit, CSerialPort::FlowControl::XonXoffFlowControl);

		//char sBuf[] = "BL";
		//port.Write(sBuf, strlen(sBuf));
		//port.TransmitChar('ON');

		//port.Flush();

		//port.Close();
	
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
	///blur(src_gray, src_gray, Size(100, 100));

	/// Create Window

	CSerialPort port;
	port.Open(4, 115200, CSerialPort::Parity::NoParity, 8, CSerialPort::StopBits::OneStopBit, CSerialPort::FlowControl::XonXoffFlowControl);
	//char rxBuf[1];
	//char sBuf[] = "(";
	//port.Write("(", 1);
	//char rxBuf[5];
	//port.Read(rxBuf, 1);
	//cout << rxBuf[0] << endl;
	//port.Write("1", 1);
	//port.Read(rxBuf, 1);
	//cout << rxBuf[0] << endl;
	//char sBuf[] = to_string(contours[i][j].x).c_str();
	//port.Write(sBuf, strlen(sBuf));
	//port.Write(",", 1);
	//port.Read(rxBuf, 1);
	//cout << rxBuf[0] << endl;
	//port.Write("8", 1);
	//port.Read(rxBuf, 1);
	//cout << rxBuf[0] << endl;
	//char sBuf[] = { contours[i][j].y };
	//port.Write(sBuf, strlen(sBuf));
	//port.Write(")", 1);
	//port.Read(rxBuf, 1);
	//cout << rxBuf[0] << endl;
	//port.TransmitChar('ON');

	//char rxBuf[1];
	//port.Read(rxBuf, 1);
	//for (int i = 0; i < 1; i++) 
	//cout << rxBuf[0] << endl;
	

	//port.Read(rxBuf, 5);
	//cout << rxBuf;
	//for (int i = 0; i < 3; i = i + 1)
	//{ 
	//cout << rxBuf[i];
	//}
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
	///int rows =  sizeof contours / sizeof contours[0]; // 2 rows  
	///cout << contours.size() << endl;
	
	//std::for_each(std::begin(int)(contours[1][1].x), std::end(contours[1][1].x),
	// Now add the last element with no delimiter 
	/*
	port.Write("(", 1);
	string xcoor= to_string({ contours[1][1].x });
	const char* x = { xcoor.c_str()};
	//char * x = new (&contours[1][1].x) char[sizeof(contours[1][1].x)];
	port.Write(x, (strlen(x)));
	//for (i = 0; i < strlen(x); i++) cout << x[i] << endl;
	cout << x << endl;
	//cout << contours[1][1].x << endl;
	//for (i = 0; i < strlen(rxBuf); i++) 
	string ycoor = to_string({ contours[1][1].y });
	const char* y = { ycoor.c_str() };
	cout << y << endl;
	port.Write(",", 1);
	port.Write(y, (strlen(y)));
	port.Write(")", 1);

	port.Read(rxBuf, 10);
	cout << rxBuf;
	*/

	//const char *g = f;
	//char y = 'x';
	//cout << f;
	//static_cast<DWORD>(strlen(f))
	//port.Read(rxBuf, 10);
	//for (i = 0; i < strlen(rxBuf); i++) cout << rxBuf[i];
	//cout << static_cast<DWORD>(strlen(f));
	//port.Write((char)vts, (int)sizeof(contours[1][1].x));
	///int cols = sizeof contours[0] / sizeof(int);
	//port.Read(rxBuf, 1);
	//cout << rxBuf;
	String imageName("../data/download.png"); // by default
	src = imread(imageName, IMREAD_COLOR);
	Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
	src = src > 1;
	//namedWindow("Source", 1);
	//imshow("Source", src);
	Scalar color(40, 40, 40);

	for (i = 0; i < contours.size(); i++)
	{
		for (j = 0; j < contours[i].size(); j++)
		{
			//contours[i][j].x = contours[i][j].x*con1;
			//contours[i][j].y = contours[i][j].y*con2;
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
			//rxBuf.resize(10);
			while (pass) {
				port.TransmitChar('w');
				memset(rxBuf, 0, sizeof(rxBuf));
				port.Read(rxBuf, 13);
				cout << "Arduino : " << rxBuf;
				cout << endl;
				//cout << "waiting" << endl;
				
				
				if (rxBuf[0] == (char)'z') { pass = false; memset(rxBuf, 0, sizeof(rxBuf)); }
				
			}
			//if (j % 6 == 0) cout << endl; else cout << ",";
			//cout << contours[i].size() << endl;
			//imshow("contour1", contours[i][j]);
		}
	}

	//drawContours(dst, contours, 2, color, CV_FILLED, 8, hierarchy);
	//cout << contours.size() << endl;

	//namedWindow("Components", 1);
	//imshow("Components", dst);
	//cout << contours[6][0];
}







