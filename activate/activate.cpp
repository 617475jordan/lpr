#include "activate.h"
#include <vector>
#include <string>
#include <iostream>

struct secretData
{
	int m_first = 1;
	int m_second = 2;
	int m_third = 3;
	int m_forth = 4;
};


std::vector<string> m_vecSecert;

activate::activate(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

activate::~activate()
{

}

void activate::initalize()
{

}

bool activate::m_key(std::vector<string> m_vecKey)
{
	return true;
}

std::vector<std::string> activate::getActivateData()
{
	std::vector<string> m_vecText;
	m_vecText.push_back(ui.firstLineEdit->text().toStdString());
	m_vecText.push_back(ui.secondlineEdit->text().toStdString());
	m_vecText.push_back(ui.thirdLineEdit->text().toStdString());
	m_vecText.push_back(ui.forthLineEdit->text().toStdString());
	return m_vecText;
}

bool activate::checkKey(std::string m_strInput, int Value)
{
	/*char Key[4];
	for (int i = 0; i < 4; i++)
	{
		Key[i] = m_strInput[i];
	}*/
	int Sum = 0;
	if (Value == 0)
	{
		if (m_strInput=="WOYE")
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		for (int i = 0; i < m_strInput.length(); i++)
			Sum = Sum + m_strInput[i];
		if (Sum % 15 == Value)
			return true;
		else
			return false;
	}
}

void activate::on_activatePushButton_clicked()
{
	secretData m_secretData;
	std::vector<string> m_vecText = getActivateData();
	if (m_vecText[0].length() == 4 && m_vecText[1].length() == 4 && m_vecText[2].length() == 4 && m_vecText[3].length() == 4)
	{
		/*const char *m_firstText, *m_secondText, *m_thirdText, *m_forthText;
		m_firstText = m_vecText[0].c_str();
	

	m_secondText= m_vecText[1].c_str();
		m_thirdText= m_vecText[2].c_;
		m_forthText = m_vecText[3].c_str();*/
		if (checkKey(m_vecText[0], 0) == true && checkKey(m_vecText[1], m_secretData.m_first) == true && checkKey(m_vecText[2], m_secretData.m_second) == true 
			&& checkKey(m_vecText[3], m_secretData.m_third) == true)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("激活成功"));

		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("激活失败"));
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("对不起！您的注册号输入错误，注册失败"));
	}
}
