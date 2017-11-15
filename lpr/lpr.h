#pragma once

#include "easypr/util/switch.hpp"
#include "easypr/core/plate_recognize.h"
//#include "easypr/core/plate.hpp"
#if _DEBUG
#pragma  comment(lib,"..\\lib\\debug\\libeasyprd.lib")
#else
#pragma  comment(lib,"..\\lib\\release\\libeasypr.lib")
#endif

struct  plateData
{
	std::vector<string> m_plateColor /*= NULL*/;
	std::vector<string> m_plateLicense /*= NULL*/;
};
using namespace easypr;
class lpr
{
public:
	lpr();
	~lpr();
	plateData                  test_plate_recognize(cv::Mat src, easypr::CPlateRecognize *pr);
	std::vector<cv::Mat>       test_plate_locate(cv::Mat m_matImg, CPlateLocate *plate);
	std::vector<cv::Mat>       test_plate_judge(cv::Mat m_matImg, CPlateLocate *lo);
	std::vector<cv::Mat>       test_plate_detect(cv::Mat m_matImg, CPlateDetect *pd);
};

