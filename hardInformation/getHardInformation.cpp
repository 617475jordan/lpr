#include "getHardInformation.h"
#pragma comment(lib,"Iphlpapi.lib") 

getHardInformation::getHardInformation()
{
}


getHardInformation::~getHardInformation()
{
}

physicicalData getHardInformation::getHardResult()
{
	physicicalData m_physicicalData;
	m_physicicalData.clear();

	/************获取Mac序列号**************/
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	int netCardNum = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}

	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			//cout <<  << ++netCardNum << endl;
			//cout << "Macaddress:";
			string m_strMacAddress;
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				char m_tenToSixteen[100];
				sprintf(m_tenToSixteen, "%x", pIpAdapterInfo->Address[i]);
				if (i < pIpAdapterInfo->AddressLength - 1)
				{
					m_strMacAddress = m_strMacAddress + m_tenToSixteen + "-";
				}
				else
				{
					m_strMacAddress = m_strMacAddress + m_tenToSixteen;
				}
			}
			m_physicicalData.m_macAddress.push_back(m_strMacAddress);
			m_strMacAddress.clear();
			pIpAdapterInfo = pIpAdapterInfo->Next;

		}

	}
	/************获取CPU序列号**************/
	__int32 deBuf[4];
	__cpuidex(deBuf, 01, 0);
	char result[100];
	sprintf(result, "%.8x%.8x", deBuf[3], deBuf[0]);
	m_physicicalData.m_cpuId.push_back(result);


	SYSTEM_INFO *siSysInfo=new SYSTEM_INFO();
	// Copy the hardware information to the SYSTEM_INFO structure. 
	GetSystemInfo(siSysInfo);
	// Display the contents of the SYSTEM_INFO structure. 
	char m_charCpuType[100];
	sprintf(m_charCpuType, "%u\n", siSysInfo->dwProcessorType);
	
	m_physicicalData.m_cpuType = m_charCpuType;
	siSysInfo = NULL;
	delete[] siSysInfo;

	m_physicicalData.m_cpuFreq = CpuFrequency();
	
	return m_physicicalData;
}

double getHardInformation::CpuFrequency()
{
	//On a multiprocessor machine, it should not matter which processor is called.  
	//However, you can get different results on different processors due to bugs in  
	//the BIOS or the HAL. To specify processor affinity for a thread, use the SetThreadAffinityMask function.  
	HANDLE hThread = GetCurrentThread();
	SetThreadAffinityMask(hThread, 0x1);

	//主板上高精度定时器的晶振频率  
	//这个定时器应该就是一片8253或者8254  
	//在intel ich7中集成了8254  
	LARGE_INTEGER lFrequency;
	QueryPerformanceFrequency(&lFrequency);
	//printf("高精度定时器的晶振频率：%1.0fHz.\n",(double)lFrequency.QuadPart);  

	//这个定时器每经过一个时钟周期，其计数器会+1  
	LARGE_INTEGER lPerformanceCount_Start;
	QueryPerformanceCounter(&lPerformanceCount_Start);

	//RDTSC指令:获取CPU经历的时钟周期数  
	__int64 _i64StartCpuCounter = RDTSC();

	//延时长一点,误差会小一点  
	//int nTemp=100000;  
	//while (--nTemp);  
	Sleep(200);

	LARGE_INTEGER lPerformanceCount_End;
	QueryPerformanceCounter(&lPerformanceCount_End);

	__int64 _i64EndCpuCounter = RDTSC();

	//f=1/T => f=计数次数/(计数次数*T)  
	//这里的“计数次数*T”就是时间差  
	double fTime = ((double)lPerformanceCount_End.QuadPart - (double)lPerformanceCount_Start.QuadPart)
		/ (double)lFrequency.QuadPart;

	return (long)(_i64EndCpuCounter - _i64StartCpuCounter) / fTime / 1000000.0;
}




