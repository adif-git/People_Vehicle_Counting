#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
 
Mat frame; Mat gray; Mat It0; Mat It1; Mat hasil;
Mat element = getStructuringElement(MORPH_RECT, Size(33, 33), Point(-1, -1));
Mat element2 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

void main()
{
	VideoCapture cap("Video.avi");

	int cnt = 0;
	int motor= 0;
	int orang = 0;

	for (;;)
	{
		cap >> frame;
		//blur(frame, frame, Size(3, 3), Point(-1, -1));
		if (frame.empty())
			break;

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		//langkah 1
		It0 = gray.clone();
		if (cnt == 0)
		{ 
			It1 = It0.clone(); 
			cnt = 1; 
			hasil = It0.clone();
		}
		//langkah 2
		absdiff(It0, It1, hasil);
		//langkah 3
		It1 = It0.clone();
		//======================================================

		vector<vector<Point> > contours;
		vector<Vec4i>hierarchy;

		threshold(hasil, hasil, 45, 255, THRESH_BINARY);
		erode(hasil, hasil, element2);
		dilate(hasil, hasil, element);
		findContours(hasil, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		/*===================INFO=========================
		RETR_TREE --> retrieve all contour
		CHAIN_APPROX_SIMPLE --> compress horizontal,vertical, diagonal segments and leaves only their end points
		================================================*/

		vector<vector<Point> > contours_poly(contours.size());
		vector<Point2f>center(contours.size());
		vector<float>radius(contours.size());

		Mat drawing = Mat::zeros(hasil.size(), CV_8UC3);
		line(drawing, Point(drawing.cols / 2, 0), Point(drawing.cols / 2, drawing.cols), Scalar(0, 255, 0), 3);

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true); 
			//=========INFO============================== 
			//approxPolyDP --> Approximates a polygonal curve(s) with the specified precision
			//===========================================
			minEnclosingCircle(contours_poly[i], center[i], radius[i]);

			int x = (int)center[i].x;
			int y = (int)center[i].y;

			drawContours(drawing, contours_poly, (int)i, Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point());
			circle(drawing, center[i], 4, Scalar(255, 0, 0), -1, 8, 0);
			
			int area = contourArea(contours[i]);
			//printf("%d \t %d,%d \t", area, x, y);

			if (area > 23000 && area < 25000)	//hitung motor
			{
				putText(drawing, String("Motor"), Point(x, y - 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2, 8);
				if(x > 313 && x <327)
					motor++;
			}
			else if (area > 8000 && area < 15000) //hitung orang
			{
				putText(drawing, String("Orang"), Point(x, y - 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2, 8);
				if(x>316 && x<321)
					orang++;
			}
			printf("Jumlah motor = %d, Jumlah orang = %d\n", motor, orang);
		}

		imshow("Original", frame);
		imshow("Hasil Filter",hasil);
		//imshow("Result", drawing);
		if (waitKey(1) >= 0)break;
	}
}

