#pragma once
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <vector>
#include <iostream>
#pragma warning(disable:4035)  
#include <iostream>
#include <Windows.h>
using namespace std;


struct physicicalData
{
	vector<string> m_macAddress;
	vector<string> m_cpuId;
	string         m_biosID;
	string         m_hardDiskID;
	string         m_mainPlateID;
	string         m_cpuType;
	long           m_cpuFreq = -1;
	void clear()
	{
		m_cpuType.clear();
		m_cpuId.clear();
		m_macAddress.clear();
		m_biosID.clear();
		m_hardDiskID.clear();
		m_mainPlateID.clear();
		m_cpuFreq = -1;
	}
};

class getHardInformation
{
public:
	getHardInformation();
	~getHardInformation();
	physicicalData getHardResult();
private:
	double CpuFrequency();
	unsigned __int64 RDTSC()
	{
		__asm _emit 0x0F;
		__asm _emit 0x31;
	}
};

