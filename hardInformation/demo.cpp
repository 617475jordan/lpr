#include "getHardInformation.h" 
#include <ostream>
using namespace std;


int main(int argc, char* argv[])
{
	getHardInformation *m_getHardInformation=new getHardInformation();
	physicicalData m_phy;
	m_phy = m_getHardInformation->getHardResult();
	m_getHardInformation = NULL;
	delete[] m_getHardInformation;
	cout << "MACAddress:" << endl;
	for (unsigned int i = 0; i < m_phy.m_macAddress.size(); i++)
	{
		cout << "NO " << i+1 <<":";
		cout << m_phy.m_macAddress[i].c_str() << endl;
	}
	//cout <<"CPU TYPE:" <<m_phy.m_cpuType.c_str()/* << endl*/;
	cout << "CPU ID:";
	for (unsigned int i = 0; i < m_phy.m_cpuId.size(); i++)
	{
		cout << m_phy.m_cpuId[i].c_str() << endl;
	}
	cout << "Really Freq:" << m_phy.m_cpuFreq <<"Mhz"<< endl;
	cout << "CPU Information:" << m_phy.m_cpuInformation.c_str() << endl;
	cout << "Bios Information:" << m_phy.m_biosID.c_str() << endl;
	system("pause");
	return 0;
}

