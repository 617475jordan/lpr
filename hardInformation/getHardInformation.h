#pragma once
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <vector>
#include <iostream>
#pragma warning(disable:4035)  
#include <iostream>
#include <Windows.h>
#pragma comment(lib,"Iphlpapi.lib")
using namespace std;


struct physicicalData
{
	vector<string>      m_macAddress;
	/*****
	Íø¿¨Ãû×Ö£¬Íø¿¨ÃèÊö£¬Íø¿¨ÀàÐÍ
	******/
//	vector<AdapterData> m_Adapterinfo;
	vector<string>      m_cpuId;
	string              m_bios;
	string              m_hardDiskID;
	string              m_mainPlateID;
	string              m_cpuInformation;
	string              m_timeZone;
	vector<string>      m_AdapterName;
	vector<string>      m_AdapterDescription;
	vector<string>      m_AdapterType;
	long                m_cpuFreq = -1;
	void clear()
	{
		m_cpuId.clear();
		m_macAddress.clear();
		m_bios.clear();
		m_hardDiskID.clear();
		m_mainPlateID.clear();
		m_cpuFreq = -1;
		m_cpuInformation.clear();
		m_timeZone.clear();
		m_AdapterName.clear();
		m_AdapterDescription.clear();
		m_AdapterType.clear();
	}
};
struct AdapterData
{
	string m_AdapterName;
	string m_AdapterDescription;
	string m_AdapterType;
	void clear()
	{
		m_AdapterName.clear();
		m_AdapterDescription.clear();
		m_AdapterType.clear();
	}
};
class getHardInformation
{
public:
	getHardInformation();
	~getHardInformation();
	physicicalData getHardResult();
protected:
	double CpuFrequency();
	string getInformation(std::string Type);
	unsigned __int64 RDTSC()
	{
		__asm _emit 0x0F;
		__asm _emit 0x31;
	}

	bool GetAdapterState(DWORD nIndex);
	vector<AdapterData> getAdapterInfo();
};
