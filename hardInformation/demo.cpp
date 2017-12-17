#include "getHardInformation.h" 
#include <string>
#include <ostream>
using namespace std;


int main(int argc, char* argv[])
{
	getHardInformation *m_getHardInformation=new getHardInformation();
	physicicalData m_phy;
	m_phy = m_getHardInformation->getHardResult();
	m_getHardInformation = NULL;
	delete[] m_getHardInformation;
	//cout <<"CPU TYPE:" <<m_phy.m_cpuType.c_str()/* << endl*/;
	cout << "CPU ID:";
	for (unsigned int i = 0; i < m_phy.m_cpuId.size(); i++)
	{
		cout << m_phy.m_cpuId[i].c_str() << endl;
	}
	cout << "Really Freq:" << m_phy.m_cpuFreq <<"Mhz"<< endl;
	cout << "CPU Information:" << m_phy.m_cpuInformation.c_str() << endl;
	cout << "Bios Information:" << m_phy.m_bios.c_str() << endl;
	for (int i = 0;i< m_phy.m_AdapterName.size(); i++)
	{
		cout << "Adapter Num:" << i+1 << endl;
		cout << "m_AdapterName" <<  ":" << m_phy.m_AdapterName[i] << endl;
		cout << "m_Description" <<  ":" << m_phy.m_AdapterDescription[i] << endl;
		cout << "m_Type" <<  ":" << m_phy.m_AdapterType[i] << endl;
		cout << "Mac:"<<m_phy.m_macAddress[i] << endl;
	}
	//cout << "baseboard Information:" << m_phy.m_baseBoard.c_str() << endl;
	//cout << "timezone Information:" << m_phy.m_timeZone.c_str() << endl;
	system("pause");
	return 0;
}

