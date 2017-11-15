#include "lprdemo.h"
#include <alpr.h>
#include <time.h>
LPRdemo::LPRdemo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initialize();
	m_qTimer = new QTimer();
	m_qTimer->start(33);
	m_qRunTimer = new QTimer();
	connect(m_qTimer, SIGNAL(timeout()), this, SLOT(updateData()));
	connect(m_qRunTimer, SIGNAL(timeout()), this, SLOT(videoRunProcessing()));  // 时间到，读取当前摄像头信息

	
	m_qTimer = NULL;
	delete[] m_qTimer;
}

LPRdemo::~LPRdemo()
{

}

void LPRdemo::initialize()
{
	//m_getHardParameters = new getHardParameters();
	//m_getHardParameters->getParamaters();
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
	m_menuParamter.m_iSystem_X.push_back(ui.capLabel->width());
	m_menuParamter.m_iSystem_Y.push_back(ui.capLabel->height());
	m_menuParamter.m_iSystem_X.push_back(ui.screenShootLabel->width());
	m_menuParamter.m_iSystem_Y.push_back(ui.screenShootLabel->height());
	//pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);

	QString  m_QstrFont = "font-size : 14px";
	ui.colorLineEdit->setStyleSheet(m_QstrFont);
	ui.colorLabel->setStyleSheet(m_QstrFont);
	ui.licenseLineEdit->setStyleSheet(m_QstrFont);
	ui.licenseLabel->setStyleSheet(m_QstrFont);
	ui.timeCostLineEdit->setStyleSheet(m_QstrFont);
	ui.timeCostLabel->setStyleSheet(m_QstrFont);
	ui.stateLabe->setStyleSheet(m_QstrFont);
	ui.stateLineEdit->setStyleSheet(m_QstrFont);
	ui.stateComboBox->addItem(m_stateData.m_imageState);
	ui.stateComboBox->addItem(m_stateData.m_videoState);

	m_vecStrData.push_back(m_lprdata.m_us);
	m_vecStrData.push_back(m_lprdata.m_gb);
	m_vecStrData.push_back(m_lprdata.m_kr);
	m_vecStrData.push_back(m_lprdata.m_kr2);
	m_vecStrData.push_back(m_lprdata.m_sg);
		m_vecStrData.push_back(m_lprdata.m_mx);
	m_vecStrData.push_back(m_lprdata.m_au);
	m_vecStrData.push_back(m_lprdata.m_auwide);
	m_vecStrData.push_back(m_lprdata.m_eu);

	QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("初始化完成"));
	//ui.updateTimeLabel->setStyleSheet("font-size : 15px");
	//ui.timeLabel->setStyleSheet("font-size : 15px");
}

void LPRdemo::clearMenu()
{
	ui.licenseLineEdit->clear();
	ui.colorLineEdit->clear();
	ui.timeCostLineEdit->clear();
	ui.stateLineEdit->clear();
	ui.capLabel->clear();
	ui.screenShootLabel->clear();
}

cv::Mat LPRdemo::adjustMat(cv::Mat m_mat, int m_state)
{
	
	float m_dAdjustSizeThrshold;
	if (m_mat.cols >= m_mat.rows&&m_mat.cols >= m_menuParamter.m_iSystem_X[m_state])
	{
		m_dAdjustSizeThrshold = m_menuParamter.m_iSystem_X[m_state] *1.0 / m_mat.cols;
		cv::resize(m_mat, m_mat, cv::Size(m_dAdjustSizeThrshold*m_mat.cols, m_dAdjustSizeThrshold*m_mat.rows));
	}
	else  if (m_mat.rows > m_mat.cols&&m_mat.rows >= m_menuParamter.m_iSystem_Y[m_state])
	{
		m_dAdjustSizeThrshold = m_menuParamter.m_iSystem_Y[m_state] *1.0 / m_mat.rows;
		cv::resize(m_mat, m_mat, cv::Size(m_dAdjustSizeThrshold*m_mat.cols, m_dAdjustSizeThrshold*m_mat.rows));
	}
	return m_mat;
}

QImage LPRdemo::Mat2QImage(cv::Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)                             //3 channels color image
	{
		cv::cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}

	return qImg;
}

void LPRdemo::imageRun()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Model"), ".", tr("Image File (*.jpg;*.png;*.bmp)"));
	std::string m_strloadModelName = codec->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()
	cv::Mat m_matImg = cv::imread(m_strloadModelName);
	if (m_matImg.empty())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("读取失败"));
		m_matImg.release();
		return;
	}
	processing(m_matImg, m_strloadModelName);
}

void LPRdemo::videoRun()
{
	QString m_qstrFilename = QFileDialog::getOpenFileName(this,
		tr("Open Model"), ".", tr("Image File (*.mp4;*.flv;*.3gp;*avi)"));
	std::string m_strloadModelName = codec->fromUnicode(m_qstrFilename).data();//filename.toAscii().data()
	m_capture.open(m_strloadModelName);
	if (!m_capture.isOpened())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("视频读取失败"));
		m_capture.release();
		return;
	}
	m_bState = true;
	m_qRunTimer->start(33);
}

void LPRdemo::videoRunProcessing()
{
	cv::Mat m_matImg;
	m_capture >> m_matImg;
	if (m_matImg.empty())
	{
		m_qRunTimer->stop();
		m_capture.release();
		clearMenu();
		m_bState = false;
		return;
	}

	processing(m_matImg,NULL);
}

void LPRdemo::processing(cv::Mat m_matImg, std::string Path)
{
	clearMenu();
	cv::Mat m_matshow;
	QImage  m_capQImg;
	m_matshow = adjustMat(m_matImg, mainMenuState);
	m_capQImg = Mat2QImage(m_matshow);
	ui.capLabel->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上 

	m_matshow.release();

	int m_currentTime = clock();
	int m_maxArea = 0;
	m_matImg = cv::imread(Path);
	cv::Mat m_matMaxImg;
	cv::Mat  roi;
	alpr::Alpr *openalpr;
	for (int i = 0; i < m_vecStrData.size(); i++)
	{
		
		openalpr = new alpr::Alpr(m_vecStrData[i], "openalpr.conf");
		if (openalpr->isLoaded() == false)
		{
			openalpr = NULL;
			//delete[] openalpr;
			continue;
		}
		openalpr->setTopN(m_lprdata.maxPlates);
		int m_max_x = 0, m_max_y = 0;
		if (Path.length() != 0)
		{
			alpr::AlprResults results = openalpr->recognize(Path);
			if (results.plates.size() == 0)
			{
				openalpr = NULL;
				//delete[] openalpr;
				continue;
			}
			alpr::AlprPlateResult plate = results.plates[0];
			
			for (int i = 2; i < 4; i++)
			{
				if (plate.plate_points[i].x > m_max_x)
				{
					m_max_x = plate.plate_points[i].x;
				}
				if (plate.plate_points[i].y > m_max_y)
				{
					m_max_y = plate.plate_points[i].y;
				}
			}
			roi = m_matImg(cv::Rect(cv::Point(plate.plate_points[0].x, plate.plate_points[0].y), cv::Point(m_max_x, m_max_y)));
			if (roi.empty())
			{
				openalpr = NULL;
				//delete[] openalpr;
				continue;
			}
			if (m_maxArea < roi.cols*roi.rows)
			{
				m_maxArea = roi.cols*roi.rows;
				m_matMaxImg.release();
				m_matMaxImg = roi;
				//break;
			}

		}
		roi.release();
		openalpr = NULL;
		
	}
	openalpr = NULL;
	delete[] openalpr;
	cv::imwrite("output\\output.jpg", m_matMaxImg);
	m_matMaxImg.release();
	cv::Mat m_inImg = cv::imread("output\\output.jpg");
	roi = adjustMat(m_inImg, screenShootMenuState);
	m_capQImg = Mat2QImage(roi);
	ui.screenShootLabel->setPixmap(QPixmap::fromImage(m_capQImg));  // 将图片显示到label上 
	m_currentTime = clock() - m_currentTime;
	QString m_qStr;
	m_qStr = QString::number(m_currentTime, 10) + "ms";
	ui.timeCostLineEdit->setText(m_qStr);

	//if (m_plateData.m_plateLicense.size() > 0)
	//{
	//	workCursor.insertText(m_QcurrentTime);
	//	workCursor.insertBlock();
	//	m_qStr = QString::number(m_currentTime, 10) + "ms";
	//	ui.timeCostLineEdit->setText(m_qStr);
	//
	//	ui.stateLineEdit->setText(QString::fromLocal8Bit("有"));
	//}
	//else
	//{
	//	ui.stateLineEdit->setText(QString::fromLocal8Bit("无"));
	//}

	//for (int i = 0; i < m_plateData.m_plateLicense.size(); i++)
	//{
	//	std::string m_strPlateColor = m_plateData.m_plateColor[i];
	//	std::string m_strPlateLicense = m_plateData.m_plateLicense[i];
	//	/*if (m_strPlateLicense.length() == NULL || m_strPlateColor.length() == NULL)
	//	{
	//		continue;
	//	}*/
	//	const char *PlateColor = m_strPlateColor.c_str();
	//	ui.colorLineEdit->setText(QString::fromLocal8Bit(PlateColor));

	//	
	//	const char *PlateLicense = m_strPlateLicense.c_str();
	//	ui.licenseLineEdit->setText(QString::fromLocal8Bit(PlateLicense));

	//	workCursor.movePosition(QTextCursor::End);
	//	workCursor.insertText(QString::fromLocal8Bit(PlateColor) + " " + QString::fromLocal8Bit(PlateLicense));
	//	workCursor.insertBlock();

	//	QScrollBar *scrollbar = ui.outpuPlainTextEdit->verticalScrollBar();

	//	if (scrollbar)
	//	{
	//		scrollbar->setValue(scrollbar->maximum());
	//	}
	//	scrollbar = NULL;
	//	delete[] scrollbar;
	//	PlateColor = NULL;
	//	delete[] PlateColor;
	//	PlateLicense = NULL;
	//	delete[] PlateLicense;
	//	cv::waitKey(1000);
	//}
	//m_matImg.release();


}


void LPRdemo::on_openPushButton_clicked()
{
	QString  m_qstrState=ui.stateComboBox->currentText();
	if (m_qstrState == m_stateData.m_imageState)
	{
		imageRun();
	}
	else if (m_qstrState == m_stateData.m_videoState)
	{
		videoRun();
	}
	else
	{
		return;
	}
}

void LPRdemo::on_closePushButton_clicked()
{
	if (m_bState == true)
	{
		m_qRunTimer->stop();
		m_capture.release();
		clearMenu();
		m_bState = false;
	}
}

void LPRdemo::on_pausePushButton_clicked()
{

	if (m_bState == true)
	{
		if (m_bCurrentPauseState == true)
		{
			m_bCurrentPauseState = false;
			m_qRunTimer->start(33);
		}
		else
		{
			m_bCurrentPauseState = true;
			m_qRunTimer->stop();
		}
	}
	else
	{
		return;
	}
}

void LPRdemo::updateData()
{
	m_QcurrentTime.clear();
	time_t  m_currentTime;
	m_currentTime = time(0);
	char m_charCurrentTime[100];
	strftime(m_charCurrentTime, sizeof(m_charCurrentTime), "%Y-%m-%d %X %A ", localtime(&m_currentTime));
	m_QcurrentTime = QString(m_charCurrentTime);
	ui.updateTimeLabel->setText(m_QcurrentTime);
	
}
