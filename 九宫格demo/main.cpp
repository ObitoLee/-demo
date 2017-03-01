#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>

#define random(x) (rand()%x)

using namespace std;
using namespace cv;

vector<int> getRandomArray(int totalNum,int returnNum)
{
	srand(getTickCount());
	vector<int> input = *new vector<int>();
	for (int i = 0; i < totalNum; i++)
		input.push_back(i);

	vector<int> output = *new vector<int>();

	int end = totalNum;
	for (int i = 0; i < returnNum; i++)
	{
		vector<int>::iterator iter = input.begin();
		int num = random(end);
		iter = iter + num;
		output.push_back(*iter);
		input.erase(iter);
		end--;
	}
	return output;
}

class Config
{
public:
	Config(string path = "config.txt")
	{
		ifstream config(path);
		if (!config.is_open())
		{
			cerr << "�����ļ���" << path << "����ʧ�ܣ�����ƴд��\n\n";
			isOpen = false;
		}
		config >> imgPath;
		string configStr;
		config >> configStr;
		scale = atof(configStr.substr(configStr.find_last_of("=") + 1, configStr.max_size()).c_str());
		config >> configStr;
		FONT = atoi(configStr.substr(configStr.find_last_of("=") + 1, configStr.max_size()).c_str());
		config >> configStr;
		fontThickness = atoi(configStr.substr(configStr.find_last_of("=") + 1, configStr.max_size()).c_str());
		config >> configStr;
		switchNum = atoi(configStr.substr(configStr.find_last_of("=") + 1, configStr.max_size()).c_str());
	}

	bool isOK()
	{
		if (!isOpen)
			return false;
		if (scale <= 0)
		{
			cerr << "�������ű�����" << scale << "���󣬱������0\n\n";
			return false;
		}
		if (FONT > 7 && FONT != 16 || FONT < 0)
		{
			cerr << "���壺" << FONT << "�����ڣ����0~7��16��9������ѡ��\n\n";
			return false;
		}
		if (fontThickness < 0) 
		{
			cerr << "�����ϸ��" << fontThickness << "���󣬲���С��0\n\n";
			return false;
		}
		return true;
	}
	inline int fontSize()const{ return FONT == 1 || FONT == 5 ? 9 : 5; }
	inline int getSwitchNum()
	{ 
		srand(getTickCount());
		if (switchNum<=0)
			return random(5)+1;
		else
			return switchNum;
	}

	string imgPath;
	float scale;
	int FONT;
	int fontThickness;
	
private:
	bool isOpen = true;
	int switchNum;
};


void main()
{
	
	Config config("config.txt");
	if (!config.isOK())
	{
		getchar();
		return;
	}
	Mat background = imread(config.imgPath);
	if (background.empty())
	{
		cerr << "����ͼƬ���س��������ļ�" << config.imgPath <<"�Ƿ���ڣ�\n"<< endl;
		getchar();
		return;
	}

	int WIDTH = background.cols;
	int HEIGHT = background.rows;

	vector<int> unorderArray = getRandomArray(9, 9);//�����1~9
	vector<int> targetArray = getRandomArray(9, 5);//��Ҫ��������5��Ŀ��
	int count = 0;
	int aRound = config.getSwitchNum();
	while ((uchar)waitKey(1450) != 27)
	{
		Mat out = background.clone();
		
		for (int i = 0; i < 9; ++i)
		{
			char dispIndex[2];
			sprintf_s(dispIndex, "%i", unorderArray[i]);
			Point2f txtPos = Point2f(155 + 360 * (i % 3), 350 + 210 * (i / 3));
			putText(out, dispIndex, txtPos, config.FONT, config.fontSize(), Scalar(0, 0, 0), config.fontThickness);
		}

		unorderArray = getRandomArray(9, 9);
		stringstream ss;
		for (int i = 0; i < targetArray.size(); i++)
			ss << targetArray[i];
		putText(out, ss.str(), Point2f(325, 150), config.FONT, config.fontSize(), Scalar(0, 0, 0), config.fontThickness);
		count++;
		if (count == aRound)
		{
			aRound = config.getSwitchNum();
			targetArray = getRandomArray(9, 5);
			count = 0;
		}

		Mat disp;
		resize(out, disp, Size(config.scale*WIDTH, config.scale*HEIGHT));
		imshow("�Ź���", disp);
	}
}