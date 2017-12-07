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

	/************��ȡMac���к�**************/
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
	/************��ȡCPU���к�**************/
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

	//�����ϸ߾��ȶ�ʱ���ľ���Ƶ��  
	//�����ʱ��Ӧ�þ���һƬ8253����8254  
	//��intel ich7�м�����8254  
	LARGE_INTEGER lFrequency;
	QueryPerformanceFrequency(&lFrequency);
	//printf("�߾��ȶ�ʱ���ľ���Ƶ�ʣ�%1.0fHz.\n",(double)lFrequency.QuadPart);  

	//�����ʱ��ÿ����һ��ʱ�����ڣ����������+1  
	LARGE_INTEGER lPerformanceCount_Start;
	QueryPerformanceCounter(&lPerformanceCount_Start);

	//RDTSCָ��:��ȡCPU������ʱ��������  
	__int64 _i64StartCpuCounter = RDTSC();

	//��ʱ��һ��,����Сһ��  
	//int nTemp=100000;  
	//while (--nTemp);  
	Sleep(200);

	LARGE_INTEGER lPerformanceCount_End;
	QueryPerformanceCounter(&lPerformanceCount_End);

	__int64 _i64EndCpuCounter = RDTSC();

	//f=1/T => f=��������/(��������*T)  
	//����ġ���������*T������ʱ���  
	double fTime = ((double)lPerformanceCount_End.QuadPart - (double)lPerformanceCount_Start.QuadPart)
		/ (double)lFrequency.QuadPart;

	return (long)(_i64EndCpuCounter - _i64StartCpuCounter) / fTime / 1000000.0;
}




