#ifndef ACTIVATE_H
#define ACTIVATE_H

#include <QtWidgets/QMainWindow>
#include "ui_activate.h"
#include <qt_all_vs2015_x64.h>

using namespace std;
class activate : public QMainWindow
{
	Q_OBJECT

public:
	activate(QWidget *parent = 0);
	~activate();

private:
	Ui::activateClass ui;
private:
	void initalize();
	bool  m_key(std::vector<string> m_vecKey);
	std::vector<string> getActivateData();
	bool checkKey(std::string m_strInput, int Value);
	private slots:
	void on_activatePushButton_clicked();
};

#endif // ACTIVATE_H
