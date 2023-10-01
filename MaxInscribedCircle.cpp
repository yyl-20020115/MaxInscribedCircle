#include <opencv2/opencv.hpp>
#include "iostream"

using namespace std;
using namespace cv;

void find_circle() {
	Mat src = imread("shape.png");
	Mat src_gray, src_gaus, src_binary;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	GaussianBlur(src_gray, src_gaus, Size(), 1, 1);
	threshold(src_gaus, src_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src_binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		Mat  dist = Mat::zeros(src.size(), CV_32F);		//定义一个Mat对象，存放原图中每个点到该轮廓的距离，为浮点型数据
		//遍历每个点，计算该点到轮廓距离
		for (int row = 0; row < src.rows; row++)
		{
			for (int col = 0; col < src.cols; col++)
			{
				//通过点多边形检测计算获得点到轮廓距离，并存放至dist中
				dist.at<float>(row, col) = pointPolygonTest(contours[i], Point(col, row), true);
			}
		}

		//计算dist中，最大值和最小值，以及其位置坐标
		double minVal, maxVal;
		Point maxloc, minloc;
		minMaxLoc(dist, &minVal, &maxVal, &minloc, &maxloc);
		int radio = abs(maxVal);			//对最大值求绝对值，即为内接圆半径
		Point center;
		center = maxloc;			//某点与轮廓距离为最大值，则该点为内接圆圆心
		circle(src, center, radio, Scalar(0, 0, 255), 2, 8, 0);

	}
	imshow("src", src);

}
int main(int argc, char** argv)
{
	find_circle();

	Mat img = imread("fengye.jpg");
	threshold(img, img, 127, 255, THRESH_BINARY);
	cvtColor(img, img, COLOR_BGR2GRAY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(img, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(img, contours, 1, Scalar::all(255), 2, 8, hierachy);

	//这里说明下，轮廓1是圈出的枫叶
	Mat dist = Mat::zeros(img.size(), CV_32FC1);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double distance = pointPolygonTest(contours[1], Point2f(j, i), true);
			dist.at<float>(i, j) = static_cast<float>(distance);
		}
	}



	double minVal, maxVal;
	minMaxLoc(dist, &minVal, &maxVal, 0, 0, Mat());
	Mat drawImg1 = Mat::zeros(img.size(), CV_8UC3);
	int thredVal1 = 1;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double distance = dist.at<float>(i, j);
			if (distance > thredVal1) {
				drawImg1.at<Vec3b>(i, j)[0] = (uchar)(abs(distance / maxVal) * 255);
			}
			else if (distance < (thredVal1 * (-1))) {
				drawImg1.at<Vec3b>(i, j)[2] = (uchar)(abs(distance / minVal) * 255);
			}
			else {
				drawImg1.at<Vec3b>(i, j)[0] = (uchar)(abs(255 - distance));
				drawImg1.at<Vec3b>(i, j)[1] = (uchar)(abs(255 - distance));
				drawImg1.at<Vec3b>(i, j)[2] = (uchar)(abs(255 - distance));
			}
		}
	}
	imshow("drawImg1", drawImg1);
	imwrite("drawImg1.jpg", drawImg1);



	normalize(dist, dist, -255, 255, NORM_MINMAX);
	Mat drawImg2 = Mat::zeros(img.size(), CV_8UC3);
	int thredVal2 = 3;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			double distance = dist.at<float>(i, j);
			if (distance > thredVal2) {
				drawImg2.at<Vec3b>(i, j)[0] = (uchar)(distance);
			}
			else if (distance < (thredVal2 * (-1))) {
				drawImg2.at<Vec3b>(i, j)[2] = (uchar)(distance * (-1));
			}
			else {
				drawImg2.at<Vec3b>(i, j)[0] = (uchar)(255);
				drawImg2.at<Vec3b>(i, j)[1] = (uchar)(255);
				drawImg2.at<Vec3b>(i, j)[2] = (uchar)(255);
			}
		}
	}
	imshow("drawImg2", drawImg2);
	imwrite("drawImg2.jpg", drawImg2);

	imshow("img", img);

	waitKey();
	return 0;
}