#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>

#define random(x) (rand()%x)
#define WIDTH 1140
#define HEIGHT 710

using namespace std;
using namespace cv;

void ROI_AddImage(Mat &srcImg,Mat logoImage,int id)
{
	int ROIx, ROIy;
	ROIx = 75 + 360 * (id % 3);
	ROIy = 65 + 210 * (id / 3);

	Mat srcROI = srcImg(Rect(ROIx, ROIy, 270, 160));
	Mat mask = logoImage;//加载掩模（必须是灰度图）
	cvtColor(logoImage, mask,CV_BGR2GRAY);
	logoImage.copyTo(srcROI, mask);	//将掩膜拷贝到ROI
}

// void  LinearBlending(Mat &srcImg, Mat logoImage, int id)
// {
// 	int ROIx, ROIy;
// 	ROIx = 75 + 360 * (id % 3);
// 	ROIy = 65 + 210 * (id / 3);
// 
// 	//【0】定义一些局部变量
// 	double alphaValue = 0.5;
// 	double betaValue;
// 	Mat srcImage2, srcImage3, dstImage;
// 
// 	// 【1】读取图像 ( 两幅图片需为同样的类型和尺寸 )
// 	srcImage2 = imread("mogu.jpg");
// 	srcImage3 = imread("rain.jpg");
// 
// 	if (!srcImage2.data) { printf("读取srcImage2错误！ \n"); return false; }
// 	if (!srcImage3.data) { printf("读取srcImage3错误！ \n"); return false; }
// 
// 	// 【2】进行图像混合加权操作
// 	betaValue = (1.0 - alphaValue);
// 	addWeighted(srcImage2, alphaValue, srcImage3, betaValue, 0.0, dstImage);
// 
// 	// 【3】显示原图窗口
// 	imshow("<2>线性混合示例窗口【原图】", srcImage2);
// 	imshow("<3>线性混合示例窗口【效果图】", dstImage);
// }

void main()
{
	ifstream config("config.txt");
	string imgPath;
	config >> imgPath;

	vector<Mat> people;
	for (int i = 1; i < 4; ++i)
	{
		stringstream ss;
		ss << imgPath << "\\" << i << ".png";
		Mat peopleImg = imread(ss.str());
		resize(peopleImg, peopleImg, Size(270, 160));
		people.push_back(peopleImg);
	}

	vector<Mat> car;
	for (int i = 1; i < 10; ++i)
	{
		stringstream ss;
		ss << imgPath << "\\图片" << i << ".png";
		Mat carImg = imread(ss.str());
		resize(carImg, carImg, Size(270, 160));
		car.push_back(carImg);
	}

	int randomNum = 0;
	int cartoonNum = 0;
	randomNum = random(9);
	int64 tinit = getTickCount();
	srand(tinit);

	Mat img(HEIGHT, WIDTH, CV_8UC3, Scalar(0, 0, 0));
	string scaleStr;
	config >> scaleStr;

	float scale = atof(scaleStr.substr(scaleStr.find_last_of("=") + 1, scaleStr.max_size()).c_str());

	while ((uchar)waitKey(180) != 27)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (i!=randomNum)
				ROI_AddImage(img, car[i], i);
			else
				ROI_AddImage(img, people[cartoonNum%3], i);
		}

		int64 t = getTickCount() - tinit;
		cartoonNum++;

		if (1000.*t / getTickFrequency() > 1400)
		{
			cartoonNum = 0;
			tinit = getTickCount();
			randomNum = random(9);
			cout << 1000.*t / getTickFrequency() << endl;
		}
		Mat out;
		resize(img, out, Size(scale*WIDTH, scale*HEIGHT));
		imshow("九宫格", out);
	}
}