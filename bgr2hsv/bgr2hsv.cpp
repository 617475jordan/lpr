#include <opencv2.4.10_all.h>
#include <iostream>
using namespace cv;
using namespace std;


int main()
{
	//读入图像  

	char file[100];
	for (int x = 1; x <= 9; x++)
	{
		sprintf(file, "%d.jpg", x);
		cv::Mat image = imread(file);
		if (image.empty())
		{
			continue;
		}
		Mat hsvimage;

		//cout << "Size:" << image.size() << endl;
		//cout << "Type:" << image.type() << endl;

		/// 创建窗口  
		namedWindow("BranStarkRGB");
		namedWindow("BranStarkHSV");

		//dilate(image, image, (5, 5));
		cvtColor(image, hsvimage, CV_BGR2HSV);

		/// 显示图像  
		imshow("BranStarkRGB", image);
		imshow("BranStarkHSV", hsvimage);

		cout << (int)hsvimage.at<Vec3b>(0, 0).type << endl;

		vector<int> m_color(5);
		//输出一些值进行对比  
		for (int i = 0; i < image.rows; ++i)
		{
			for (int j = 0; j < image.cols; ++j)
			{
				//cout << (int)hsvimage.at<Vec3b>(i, j).val[0] << " " << (int)hsvimage.at<Vec3b>(i, j).val[1]<<" "<<(int)hsvimage.at<Vec3b>(i, j).val[2] << endl;
				
				if ((int)hsvimage.at<Vec3b>(i, j).val[2] >= 180 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 255 &&
					(int)hsvimage.at<Vec3b>(i, j).val[1] >= 0 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 40)
				{
					m_color[1]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[0] >= 11 && (int)hsvimage.at<Vec3b>(i, j).val[0] < 34 &&
					(int)hsvimage.at<Vec3b>(i, j).val[1] >= 43 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 255 &&
					(int)hsvimage.at<Vec3b>(i, j).val[2] >= 46 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 255)
				{
					m_color[2]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[0] >= 35 && (int)hsvimage.at<Vec3b>(i, j).val[0] < 88 &&
					(int)hsvimage.at<Vec3b>(i, j).val[1] >= 43 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 255 &&
					(int)hsvimage.at<Vec3b>(i, j).val[2] >= 46 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 255)
				{
					m_color[3]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[0] >= 88 && (int)hsvimage.at<Vec3b>(i, j).val[0] < 155 &&
					(int)hsvimage.at<Vec3b>(i, j).val[1] >= 43 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 255 &&
					(int)hsvimage.at<Vec3b>(i, j).val[2] >= 46 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 255)
				{
					m_color[4]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[2] < 40)
				{
					m_color[0]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[1] >= 0 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 43 &&
					(int)hsvimage.at<Vec3b>(i, j).val[2] >= 115 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 220)
				{
					m_color[1]++;
				}
				else if ((int)hsvimage.at<Vec3b>(i, j).val[1] >= 0 && (int)hsvimage.at<Vec3b>(i, j).val[1] < 43 &&
					(int)hsvimage.at<Vec3b>(i, j).val[2] >= 46 && (int)hsvimage.at<Vec3b>(i, j).val[2] < 115)
				{
					m_color[0]++;
				}
			}
		}
		int m_max = 0;
		int m_maxTmp = 0;
		for (int i = 0; i < 5; i++)
		{
			cout << m_color[i] << " ";
			if (m_max < m_color[i])
			{
				m_max = m_color[i];
				m_maxTmp = i;
			}
		}
		cout <<"ID:" <<x<<" "<<m_maxTmp << endl;
		switch (m_maxTmp)
		{
		case 0:
			cout << "黑色" << endl;
			break;
		case 1:
			cout << "白色" << endl;
			break;
		case 2:
			cout << "黄色" << endl;
			break;
		case 3:
			cout << "绿色" << endl;
			break;
		case 4:
			cout << "蓝色" << endl;
			break;
		default:
			cout << "无" << endl;
			break;
		}
		/// 等待用户按键  
		waitKey();
		destroyAllWindows();
		hsvimage.release();
		image.release();

	}
	return 0;
}
