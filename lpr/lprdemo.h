#ifndef LPRDEMO_H
#define LPRDEMO_H

#include <QtWidgets/QMainWindow>
#include "ui_lprdemo.h"
#include <qt_all_vs2015_x64.h>
#include <opencv3.1.0_all.h>
#include <alpr.h>
//#include "getHardParameters.h"
//#include  <windows.h>
struct menuParamter
{
	std::vector<int> m_iSystem_X;
	std::vector<int> m_iSystem_Y;
};
struct lprdata
{
	std::string m_us     = "us";
	std::string m_au     = "au";
	std::string m_auwide = "auwide";
	std::string m_eu     = "eu";
	std::string m_gb     = "gb";
	std::string m_kr     = "kr";
	std::string m_kr2    = "kr2";
	std::string m_mx     = "mx";
	std::string m_sg     = "sg";


	int  maxPlates       =  1;
};
struct stateData
{
	QString m_imageState = QString::fromLocal8Bit("图片模式");
	QString m_videoState = QString::fromLocal8Bit("视频模式");
};
enum  menuState
{
	mainMenuState,
	screenShootMenuState
};
class LPRdemo : public QMainWindow
{
	Q_OBJECT

public:
	LPRdemo(QWidget *parent = 0);
	~LPRdemo();
	
private:
	Ui::LPRdemoClass ui;
	QTimer                     *m_qTimer;
	QTimer                     *m_qRunTimer;
	
	menuParamter               m_menuParamter;
	lprdata                    m_lprdata;
	QTextCodec                 *codec = QTextCodec::codecForName("GB18030");
	cv::VideoCapture           m_capture;
	stateData                  m_stateData;
	bool                       m_bState = false;
	bool                       m_bCurrentPauseState = false;
	std::vector<std::string>   m_vecStrData;
	
	//getHardParameters          *m_getHardParameters;
private:
	void                       initialize();
	void                       clearMenu();
	cv::Mat                    adjustMat(cv::Mat m_mat,int m_state);
	QImage                     Mat2QImage(cv::Mat cvImg);

	QString                    m_QcurrentTime;
	void                       imageRun();
	void                       videoRun();
	void                       processing(cv::Mat m_matImg,std::string Path);

private slots:
void                           on_openPushButton_clicked();
void                           on_closePushButton_clicked();
void                           on_pausePushButton_clicked();
void                           videoRunProcessing();
void                           updateData();
};

#endif // LPRDEMO_H
