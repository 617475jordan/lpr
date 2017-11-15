#include "lpr.h"


lpr::lpr()
{
}


lpr::~lpr()
{
}

plateData lpr::test_plate_recognize(cv::Mat src, easypr::CPlateRecognize *pr)
{
	vector<easypr::CPlate> plateVec;

	int result = pr->plateRecognize(src, plateVec);
	//int result = pr.plateRecognizeAsText(src, plateVec);
	plateData m_plateData;
	if (result == 0)
	{
		size_t num = plateVec.size();

		for (size_t j = 0; j < num; j++)
		{
			std::string m_plate = plateVec[j].getPlateStr();
			std::string m_strPlateColor;
			std::string m_strPlateNum;
			for (int i = 0; i < plateVec[j].getPlateStr().size(); i++)
			{
				if (i < 4)
				{
					m_strPlateColor = m_strPlateColor + plateVec[j].getPlateStr()[i];
				}
				else if (i>4)
				{
					m_strPlateNum = m_strPlateNum + plateVec[j].getPlateStr()[i];
				}
			}
			m_plateData.m_plateColor.push_back(m_strPlateColor);
			m_plateData.m_plateLicense.push_back(m_strPlateNum);
			//cout << "plateRecognize: " << plateVec[j].getPlateStr() << endl;
		}
	}
	pr = NULL;
	delete[] pr;
	return m_plateData;
}

std::vector<cv::Mat> lpr::test_plate_locate(cv::Mat m_matImg, CPlateLocate *plate)
{
	/*cout << "test_plate_locate" << endl;

	const string file = "resources/image/plate_locate.jpg";

	cv::Mat m_matImg = imread(file);*/

	vector<cv::Mat> resultVec;
	//CPlateLocate plate;
	//plate.setDebug(0);
	//plate.setLifemode(true);

	int result = plate->plateLocate(m_matImg, resultVec);
	plate = NULL;
	delete[] plate;
	return resultVec;
	/*if (result == 0) {
		size_t num = resultVec.size();
		for (size_t j = 0; j < num; j++) {
		cv::Mat resultMat = resultVec[j];
		imshow("plate_locate", resultMat);
		waitKey(0);
		}
		destroyWindow("plate_locate");
		}

		return result;*/
}

std::vector<cv::Mat> lpr::test_plate_judge(cv::Mat m_matImg, CPlateLocate *lo)
{
	/*cout << "test_plate_judge" << endl;

	cv::Mat m_matImg = imread("resources/image/plate_judge.jpg");*/

	vector<cv::Mat> matVec;
	vector<cv::Mat> resultVec;

	//CPlateLocate lo;
	lo->setDebug(1);
	lo->setLifemode(true);

	int resultLo = lo->plateLocate(m_matImg, matVec);

	//if (0 != resultLo) return -1;

	//cout << "plate_locate_img" << endl;
	size_t num = matVec.size();
	/*for (size_t j = 0; j < num; j++) {
		Mat resultMat = matVec[j];
		imshow("plate_judge", resultMat);
		waitKey(0);
		}
		destroyWindow("plate_judge");
		*/
	int resultJu = PlateJudge::instance()->plateJudge(matVec, resultVec);

	/*if (0 != resultJu) return -1;

	cout << "plate_judge_img" << endl;
	num = resultVec.size();
	for (size_t j = 0; j < num; j++) {
		Mat resultMat = resultVec[j];
		imshow("plate_judge", resultMat);
		waitKey(0);
	}
	destroyWindow("plate_judge");*/
	lo = NULL;
	delete[] lo;
	return resultVec;
}

std::vector<cv::Mat> lpr::test_plate_detect(cv::Mat m_matImg, CPlateDetect *pd)
{
	/*cout << "test_plate_detect" << endl;

	cv::Mat m_matImg = imread("resources/image/plate_detect.jpg");*/

	vector<CPlate> resultVec;
	vector<cv::Mat>m_vecMat;
	//CPlateDetect pd;
	pd->setPDLifemode(true);
	

	int result = pd->plateDetect(m_matImg, resultVec);
	for (int i = 0; i < resultVec.size(); i++)
	{
		m_vecMat.push_back(resultVec[i].getPlateMat());
	}
	pd = NULL;
	delete[] pd;
	return m_vecMat;

}
