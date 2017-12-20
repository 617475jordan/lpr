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


struct hardwareInfo
{
	vector<string>      m_macAddress;
	/*****
	网卡名字，网卡描述，网卡类型
	******/
	//	vector<AdapterData> m_Adapterinfo;

	string              m_bios;


	vector<string>      m_AdapterName;
	vector<string>      m_AdapterDescription;
	vector<string>      m_AdapterType;


	long                m_cpuFreq = -1;
	vector<string>      m_cpuId;
	string              m_cpuInformation;
	int                 m_ProcessorArchitecture;//处理器架构
	int                 m_dwActiveProcessorMask;//处理器掩码
	int                 m_dwNumberOfProcessors;//处理器数量
	int                 m_wProcessorLevel;//处理器级别
	void clear()
	{
		m_cpuId.clear();
		m_macAddress.clear();
		m_bios.clear();
		m_cpuFreq = -1;
		m_cpuInformation.clear();
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
	hardwareInfo              getHardwareInfo();
protected:
	double                    cpuFrequency();
	string                    getInformation(std::string Type);
	unsigned __int64 RDTSC()
	{
		__asm _emit 0x0F;
		__asm _emit 0x31;
	}

	bool                     getAdapterState(DWORD nIndex);
	void                     getAdapterInfo();
protected:
	hardwareInfo             m_hardwareInfo;
};
