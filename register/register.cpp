#include "register.h"
#include "password.h"
#include <time.h>
#include<fstream>

Register::Register(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initalize();
}

Register::~Register()
{

}

void Register::initalize()
{
	std::ifstream m_infile("data\\data.index", std::ios::in );
	if (!m_infile.is_open())
	{
		m_infile.close();
		std::ofstream m_outfile("data\\data.index", std::ios::out);
		m_outfile.close();
	}
	else
	{
		std::string m_strLineData;
		while (getline(m_infile, m_strLineData))
		{
			m_passwordData.push_back(m_strLineData);
		}
		m_infile.close();
	}
}

std::string Register::currentTime()
{
	time_t  m_currentTime;
	m_currentTime = time(0);
	char m_charCurrentTime[100];
	strftime(m_charCurrentTime, sizeof(m_charCurrentTime), "%Y-%m-%d %X %A ", localtime(&m_currentTime));
	return m_charCurrentTime;
}

void Register::outputShow(QString m_qstr)
{
	
	ui.outputLineEdit->clear();
	ui.outputLineEdit->setText(m_qstr);
	auto workCursor = ui.outputPlainTextEdit->textCursor();
	workCursor.movePosition(QTextCursor::End);

	workCursor.insertText(m_qstr);
	workCursor.insertBlock();

	QScrollBar *scrollbar = ui.outputPlainTextEdit->verticalScrollBar();

	if (scrollbar)
	{
		scrollbar->setValue(scrollbar->maximum());
	}
	scrollbar = NULL;
	delete[] scrollbar;
}

bool Register::outPutPassword(std::string m_password)
{
	std::ofstream m_outfile("data\\data.index",  std::ios_base::app);
	if (!m_outfile.is_open())
	{
		m_outfile.open("data\\data.index", std::ios::out);
	}
	m_outfile << m_password << endl;
	m_outfile.close();
	return true;
}

void Register::on_clickedPushButton_clicked()
{
	password *m_password = new password();
	std::vector<int> m_inputData;
	m_inputData = m_password->createPassword();
	std::string m_outPutPassword;
	if (m_inputData.size() < 16)
	{
		m_password = NULL;
		delete[] m_password;
		return;
	}
	for (int i = 0; i < m_inputData.size(); i++)
	{
		if (i>0 && i % 4 == 0)
		{
			m_outPutPassword = m_outPutPassword + "-";
		}

		char m_tmpChar = (char)m_inputData[i];
		m_outPutPassword = m_outPutPassword + &m_tmpChar;
	}
	for (int i = 0; i < m_passwordData.size(); i++)
	{
		if (m_outPutPassword == m_passwordData[i])
		{
			QMessageBox::information(this, QString::fromLocal8Bit("友情提示"), QString::fromLocal8Bit("激活码无效"));
			return;
		}
	}
	if (!outPutPassword(m_outPutPassword))
	{
		return;
	}
	QString m_qstr = QString::fromStdString(m_outPutPassword);
	outputShow(m_qstr);
	m_password = NULL;
	delete[] m_password;
}
