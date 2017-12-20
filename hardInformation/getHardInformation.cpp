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
//#pragma comment(lib,"Iphlpapi.lib") 
#define _WIN32_DCOM  
#include <comdef.h>  
#include <Wbemidl.h>  

#pragma comment(lib, "wbemuuid.lib")

getHardInformation::getHardInformation()
{

	m_hardwareInfo.clear();
}


getHardInformation::~getHardInformation()
{
}

hardwareInfo getHardInformation::getHardwareInfo()
{

	/************��ȡCPU���к�**************/
	__int32 deBuf[4];
	__cpuidex(deBuf, 01, 0);
	char result[100];
	sprintf(result, "%.8X%.8X", deBuf[3], deBuf[0]);
	m_hardwareInfo.m_cpuId.push_back(result);

	/*
	��ȡcpuʵ��Ƶ�ʣ���ȡbios��Ϣ����ȡϵͳ�趨��cpu��Ϣ
	*/
	m_hardwareInfo.m_cpuFreq = cpuFrequency();
	m_hardwareInfo.m_bios = getInformation("Win32_BIOS");
	m_hardwareInfo.m_cpuInformation = getInformation("Win32_Processor");

	/*for (int i = 0; i < getAdapterInfo().size(); i++)
	{
	m_physicicalData.m_AdapterName.push_back(getAdapterInfo()[i].m_AdapterName);
	m_physicicalData.m_AdapterType.push_back(getAdapterInfo()[i].m_AdapterType);
	m_physicicalData.m_AdapterDescription.push_back(getAdapterInfo()[i].m_AdapterDescription);
	m_physicicalData.m_macAddress.push_back(getAdapterInfo()[i].m_macAddress);
	}*/
	getAdapterInfo();
	//getInformation("Win32_TimeZone");

	SYSTEM_INFO  sysInfo;   //�ýṹ������˵�ǰ���������Ϣ�����������ϵ�ṹ�����봦���������͡�ϵͳ�����봦������������ҳ��Ĵ�С�Լ�������Ϣ��  
	OSVERSIONINFOEX osvi;
	GetSystemInfo(&sysInfo);
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	m_hardwareInfo.m_ProcessorArchitecture = sysInfo.wProcessorArchitecture;
	m_hardwareInfo.m_wProcessorLevel = sysInfo.wProcessorLevel;
	m_hardwareInfo.m_dwActiveProcessorMask = sysInfo.dwActiveProcessorMask;
	m_hardwareInfo.m_dwNumberOfProcessors = sysInfo.dwNumberOfProcessors;
	return m_hardwareInfo;
}

double getHardInformation::cpuFrequency()
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
		//wcout << "  Name : " << vtProp.bstrVal << endl;
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

bool getHardInformation::getAdapterState(DWORD nIndex)
{
	MIB_IFROW miInfo;   // ��Ż�ȡ���� Adapter ����
	memset(&miInfo, 0, sizeof(MIB_IFROW));
	miInfo.dwIndex = nIndex;   // dwIndex ����Ҫ��ȡ�� Adapter ������
	if (GetIfEntry(&miInfo) != NOERROR)
	{
		//printf("ErrorCode = %d\n", GetLastError());
		return false;
	}
	if (miInfo.dwOperStatus == IF_OPER_STATUS_NON_OPERATIONAL || miInfo.dwOperStatus == IF_OPER_STATUS_UNREACHABLE
		|| miInfo.dwOperStatus == IF_OPER_STATUS_DISCONNECTED || miInfo.dwOperStatus == IF_OPER_STATUS_CONNECTING)
	{
		return false;
	}
	else if (miInfo.dwOperStatus == IF_OPER_STATUS_OPERATIONAL || miInfo.dwOperStatus == IF_OPER_STATUS_CONNECTED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void getHardInformation::getAdapterInfo()
{
	//vector<AdapterData> m_vecAdapterData;
	//AdapterData m_AdapterData;
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	// PIP_ADAPTER_INFO �ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIPAdapterInfo = new IP_ADAPTER_INFO();
	PIP_ADAPTER_INFO pAdapter = NULL;
	// �õ��ṹ���С , ���� GetAdaptersInfo ����
	unsigned long ulSize = sizeof(IP_ADAPTER_INFO);
	// ���� GetAdaptersInfo ���� , ��� pIpAdapterInfo ָ����� , ���� ulSize ��������һ��������Ҳ��һ�������
	int nRstCode = GetAdaptersInfo(pIPAdapterInfo, &ulSize);
	// ��¼��������
	int nNetCardNum = 0;
	// ��¼ÿ�������ϵ� IP ��ַ����
	int nIPNumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRstCode)
	{
		// ����������ص��� ERROR_BUFFER_OVERFLOW
		// ��˵�� GetAdaptersInfo �������ݵ��ڴ�ռ䲻�� , ͬʱ�䴫�� ulSize , ��ʾ��Ҫ�Ŀռ��С
		// ��Ҳ��˵��Ϊʲô ulSize ����һ��������Ҳ��һ�������
		// �ͷ�ԭ�����ڴ�ռ�
		delete pIPAdapterInfo;
		// ���������ڴ�ռ������洢����������Ϣ
		pIPAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[ulSize];
		// �ٴε��� GetAdaptersInfo ���� , ��� pIpAdapterInfo ָ�����
		nRstCode = GetAdaptersInfo(pIPAdapterInfo, &ulSize);
	}
	if (ERROR_SUCCESS == nRstCode)
	{
		// ���������Ϣ , �����ж����� , ���ͨ��ѭ��ȥ�ж�
		pAdapter = pIPAdapterInfo;
		while (pAdapter)
		{
			//cout << "�������� : " << ++nNetCardNum << endl;
			//cout << "�������� : " << pAdapter->AdapterName << endl;
			//cout << "�������� : " << pAdapter->Description << endl;
			//m_AdapterData.clear();
			m_hardwareInfo.m_AdapterName.push_back(pAdapter->AdapterName);
			m_hardwareInfo.m_AdapterDescription.push_back(pAdapter->Description);
			string m_type;
			switch (pAdapter->Type)
			{
			case MIB_IF_TYPE_OTHER:
				m_type = "OTHER";
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_type = "ETHERNET";
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_type = "TOKENRING";
				break;
			case MIB_IF_TYPE_FDDI:
				m_type = "FDDI";
				break;
			case MIB_IF_TYPE_PPP:
				m_type = "PPP";
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_type = "LOOPBACK";
				break;
			case MIB_IF_TYPE_SLIP:
				m_type = "SLIP";
				break;
			default:
				break;
			}
			m_hardwareInfo.m_AdapterType.push_back(m_type);
			//cout << "����MAC��ַ : ";
			string m_strMacAddress;
			for (DWORD i = 0; i < pAdapter->AddressLength; i++)
			{
				char m_tenToSixteen[100];
				sprintf(m_tenToSixteen, "%X", pAdapter->Address[i]);
				if (i < pAdapter->AddressLength - 1)
				{
					m_strMacAddress = m_strMacAddress + m_tenToSixteen + "-";
				}
				else
				{
					m_strMacAddress = m_strMacAddress + m_tenToSixteen;
				}
			}
			//m_AdapterData.m_macAddress=m_strMacAddress;
			m_hardwareInfo.m_macAddress.push_back(m_strMacAddress);
			pAdapter = pAdapter->Next;
			//cout << "--------------------------------------------------------------------" << endl;
		}
	}
	// �ͷ��ڴ�ռ�
	if (pIPAdapterInfo != NULL)
	{
		delete[] pIPAdapterInfo;
		pIPAdapterInfo = NULL;
	}
	//getchar();
}

