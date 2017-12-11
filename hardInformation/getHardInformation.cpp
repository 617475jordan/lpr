//Win32_Processor                        // CPU ������
//Win32_PhysicalMemory                   // �����ڴ�
//Win32_Keyboard                         // ����
//Win32_PointingDevice                   // �������豸�������
//Win32_DiskDrive                        // Ӳ��������
//Win32_CDROMDrive                       // ����������
//Win32_BaseBoard                        // ����
//Win32_BIOS                             // BIOS оƬ
//Win32_ParallelPort                     // ����
//Win32_SerialPort                       // ����
//Win32_SoundDevice                      // ��ý������
//Win32_USBController                    // USB ������
//Win32_NetworkAdapter                   // ����������
//Win32_NetworkAdapterConfiguration      // ��������������
//Win32_Printer                          // ��ӡ��
//Win32_PrinterConfiguration             // ��ӡ������
//Win32_PrintJob                         // ��ӡ������
//Win32_TCPIPPrinterPort                 // ��ӡ���˿�
//Win32_POTSModem                        // MODEM
//Win32_POTSModemToSerialPort            // MODEM �˿�
//Win32_DesktopMonitor                   // ��ʾ��
//Win32_VideoController                  // �Կ�ϸ�ڡ�
//Win32_VideoSettings                    // �Կ�֧�ֵ���ʾģʽ��
//Win32_TimeZone                         // ʱ��
//Win32_SystemDriver                     // ��������
//Win32_DiskPartition                    // ���̷���
//Win32_LogicalDisk                      // �߼�����
//Win32_LogicalMemoryConfiguration       // �߼��ڴ�����
//Win32_PageFile                         // ϵͳҳ�ļ���Ϣ
//Win32_PageFileSetting                  // ҳ�ļ�����
//Win32_BootConfiguration                // ϵͳ��������
//Win32_OperatingSystem                  // ����ϵͳ��Ϣ
//Win32_StartupCommand                   // ϵͳ�Զ���������
//Win32_Service                          // ϵͳ��װ�ķ���
//Win32_Group                            // ϵͳ������
//Win32_GroupUser                        // ϵͳ���ʺ�
//Win32_UserAccount                      // �û��ʺ�
//Win32_Process                          // ϵͳ����
//Win32_Thread                           // ϵͳ�߳�
//Win32_Share                            // ����
//Win32_NetworkClient                    // �Ѱ�װ������ͻ���
//Win32_NetworkProtocol    

#include "getHardInformation.h"
#pragma comment(lib,"Iphlpapi.lib") 
#define _WIN32_DCOM  
#include <comdef.h>  
#include <Wbemidl.h>  

#pragma comment(lib, "wbemuuid.lib")  
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

	m_physicicalData.m_cpuFreq = CpuFrequency();
	m_physicicalData.m_biosID = getInformation("Win32_BIOS");
	m_physicicalData.m_cpuInformation = getInformation("Win32_Processor");
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

string getHardInformation::getInformation(std::string Type)
{
	string m_cpuInforamtion;
	HRESULT hres;
	// Step 1: --------------------------------------------------  
	// Initialize COM. ------------------------------------------  

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
	/*	cout << "Failed to initialize COM library. Error code = 0x"
			<< hex << hres << endl;*/
		return m_cpuInforamtion;                  // Program has failed.  
	}

	// Step 2: --------------------------------------------------  
	// Set general COM security levels --------------------------  

	hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication  
		NULL,                        // Authentication services  
		NULL,                        // Reserved  
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication   
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation    
		NULL,                        // Authentication info  
		EOAC_NONE,                   // Additional capabilities   
		NULL                         // Reserved  
		);


	if (FAILED(hres))
	{
		/*cout << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;*/
		CoUninitialize();
		return m_cpuInforamtion;                    // Program has failed.  
	}

	// Step 3: ---------------------------------------------------  
	// Obtain the initial locator to WMI -------------------------  

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		/*cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << endl;*/
		CoUninitialize();
		return m_cpuInforamtion;                 // Program has failed.  
	}

	// Step 4: -----------------------------------------------------  
	// Connect to WMI through the IWbemLocator::ConnectServer method  

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with  
	// the current user and obtain pointer pSvc  
	// to make IWbemServices calls.  
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace  
		NULL,                    // User name. NULL = current user  
		NULL,                    // User password. NULL = current  
		0,                       // Locale. NULL indicates current  
		NULL,                    // Security flags.  
		0,                       // Authority (for example, Kerberos)  
		0,                       // Context object   
		&pSvc                    // pointer to IWbemServices proxy  
		);

	if (FAILED(hres))
	{
		/*cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;*/
		pLoc->Release();
		CoUninitialize();
		return m_cpuInforamtion;                // Program has failed.  
	}

	//cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------  
	// Set security levels on the proxy -------------------------  

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set  
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx  
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx  
		NULL,                        // Server principal name   
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx   
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx  
		NULL,                        // client identity  
		EOAC_NONE                    // proxy capabilities   
		);

	if (FAILED(hres))
	{
		/*cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;*/
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return m_cpuInforamtion;               // Program has failed.  
	}

	// Step 6: --------------------------------------------------  
	// Use the IWbemServices pointer to make requests of WMI ----  

	// For example, get the name of the operating system  
	IEnumWbemClassObject* pEnumerator = NULL;
	char m_hardwareType[200] = "SELECT * FROM ";
	int m_length = strlen(m_hardwareType);
	for (int i = 0; i < Type.length(); i++)
	{
		m_hardwareType[i + m_length] = Type[i];
	}
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(m_hardwareType),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		/*cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< hex << hres << endl;*/
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return m_cpuInforamtion;               // Program has failed.  
	}

	// Step 7: -------------------------------------------------  
	// Get the data from the query in step 6 -------------------  

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property  
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		//wcout << " bios Name : " << vtProp.bstrVal << endl;
		m_cpuInforamtion = (_bstr_t)vtProp.bstrVal;
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup  
	// ========  

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return m_cpuInforamtion;   // Program successfully completed.  
}



