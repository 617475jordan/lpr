//Win32_Processor                        // CPU 处理器
//Win32_PhysicalMemory                   // 物理内存
//Win32_Keyboard                         // 键盘
//Win32_PointingDevice                   // 点输入设备，如鼠标
//Win32_DiskDrive                        // 硬盘驱动器
//Win32_CDROMDrive                       // 光盘驱动器
//Win32_BaseBoard                        // 主板
//Win32_BIOS                             // BIOS 芯片
//Win32_ParallelPort                     // 并口
//Win32_SerialPort                       // 串口
//Win32_SoundDevice                      // 多媒体设置
//Win32_USBController                    // USB 控制器
//Win32_NetworkAdapter                   // 网络适配器
//Win32_NetworkAdapterConfiguration      // 网络适配器设置
//Win32_Printer                          // 打印机
//Win32_PrinterConfiguration             // 打印机设置
//Win32_PrintJob                         // 打印机任务
//Win32_TCPIPPrinterPort                 // 打印机端口
//Win32_POTSModem                        // MODEM
//Win32_POTSModemToSerialPort            // MODEM 端口
//Win32_DesktopMonitor                   // 显示器
//Win32_VideoController                  // 显卡细节。
//Win32_VideoSettings                    // 显卡支持的显示模式。
//Win32_TimeZone                         // 时区
//Win32_SystemDriver                     // 驱动程序
//Win32_DiskPartition                    // 磁盘分区
//Win32_LogicalDisk                      // 逻辑磁盘
//Win32_LogicalMemoryConfiguration       // 逻辑内存配置
//Win32_PageFile                         // 系统页文件信息
//Win32_PageFileSetting                  // 页文件设置
//Win32_BootConfiguration                // 系统启动配置
//Win32_OperatingSystem                  // 操作系统信息
//Win32_StartupCommand                   // 系统自动启动程序
//Win32_Service                          // 系统安装的服务
//Win32_Group                            // 系统管理组
//Win32_GroupUser                        // 系统组帐号
//Win32_UserAccount                      // 用户帐号
//Win32_Process                          // 系统进程
//Win32_Thread                           // 系统线程
//Win32_Share                            // 共享
//Win32_NetworkClient                    // 已安装的网络客户端
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

	///************获取Mac序列号**************/
	//PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//int netCardNum = 0;
	//if (ERROR_BUFFER_OVERFLOW == nRel)
	//{
	//	delete pIpAdapterInfo;
	//	pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
	//	nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//}

	//if (ERROR_SUCCESS == nRel)
	//{
	//	while (pIpAdapterInfo)
	//	{
	//		//cout <<  << ++netCardNum << endl;
	//		//cout << "Macaddress:";
	//		string m_strMacAddress;
	//		for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
	//		{
	//			char m_tenToSixteen[100];
	//			sprintf(m_tenToSixteen, "%x", pIpAdapterInfo->Address[i]);
	//			if (i < pIpAdapterInfo->AddressLength - 1)
	//			{
	//				m_strMacAddress = m_strMacAddress + m_tenToSixteen + "-";
	//			}
	//			else
	//			{
	//				m_strMacAddress = m_strMacAddress + m_tenToSixteen;
	//			}
	//		}
	//		m_physicicalData.m_macAddress.push_back(m_strMacAddress);
	//		m_strMacAddress.clear();
	//		pIpAdapterInfo = pIpAdapterInfo->Next;

	//	}

	//}
	/************获取CPU序列号**************/
	__int32 deBuf[4];
	__cpuidex(deBuf, 01, 0);
	char result[100];
	sprintf(result, "%.8x%.8x", deBuf[3], deBuf[0]);
	m_physicicalData.m_cpuId.push_back(result);

	/*
	获取cpu实际频率，获取bios信息，获取系统设定的cpu信息
	*/
	m_physicicalData.m_cpuFreq = CpuFrequency();
	m_physicicalData.m_bios = getInformation("Win32_BIOS");
	m_physicicalData.m_cpuInformation = getInformation("Win32_Processor");

	for (int i = 0; i < getAdapterInfo().size(); i++)
	{
		m_physicicalData.m_AdapterName.push_back(getAdapterInfo()[i].m_AdapterName);
		m_physicicalData.m_AdapterType.push_back(getAdapterInfo()[i].m_AdapterType);
		m_physicicalData.m_AdapterDescription.push_back(getAdapterInfo()[i].m_AdapterDescription);
		m_physicicalData.m_macAddress.push_back(getAdapterInfo()[i].m_macAddress);
	}
	//m_physicicalData.m_Adapterinfo = getAdapterInfo();
	//m_physicicalData.m_timeZone = getInformation("Win32_TimeZone");
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

bool getHardInformation::GetAdapterState(DWORD nIndex)
{
	MIB_IFROW miInfo;   // 存放获取到的 Adapter 参数
	memset(&miInfo, 0, sizeof(MIB_IFROW));
	miInfo.dwIndex = nIndex;   // dwIndex 是需要获取的 Adapter 的索引
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

std::vector<AdapterData> getHardInformation::getAdapterInfo()
{
	vector<AdapterData> m_vecAdapterData;
	AdapterData m_AdapterData;
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	// PIP_ADAPTER_INFO 结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIPAdapterInfo = new IP_ADAPTER_INFO();
	PIP_ADAPTER_INFO pAdapter = NULL;
	// 得到结构体大小 , 用于 GetAdaptersInfo 参数
	unsigned long ulSize = sizeof(IP_ADAPTER_INFO);
	// 调用 GetAdaptersInfo 函数 , 填充 pIpAdapterInfo 指针变量 , 其中 ulSize 参数既是一个输入量也是一个输出量
	int nRstCode = GetAdaptersInfo(pIPAdapterInfo, &ulSize);
	// 记录网卡数量
	int nNetCardNum = 0;
	// 记录每张网卡上的 IP 地址数量
	int nIPNumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRstCode)
	{
		// 如果函数返回的是 ERROR_BUFFER_OVERFLOW
		// 则说明 GetAdaptersInfo 参数传递的内存空间不够 , 同时其传出 ulSize , 表示需要的空间大小
		// 这也是说明为什么 ulSize 既是一个输入量也是一个输出量
		// 释放原来的内存空间
		delete pIPAdapterInfo;
		// 重新申请内存空间用来存储所有网卡信息
		pIPAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[ulSize];
		// 再次调用 GetAdaptersInfo 函数 , 填充 pIpAdapterInfo 指针变量
		nRstCode = GetAdaptersInfo(pIPAdapterInfo, &ulSize);
	}
	if (ERROR_SUCCESS == nRstCode)
	{
		// 输出网卡信息 , 可能有多网卡 , 因此通过循环去判断
		pAdapter = pIPAdapterInfo;
		while (pAdapter)
		{
			//cout << "网卡数量 : " << ++nNetCardNum << endl;
			//cout << "网卡名称 : " << pAdapter->AdapterName << endl;
			//cout << "网卡描述 : " << pAdapter->Description << endl;
			m_AdapterData.clear();
			m_AdapterData.m_AdapterName=pAdapter->AdapterName;
			m_AdapterData.m_AdapterDescription=pAdapter->Description;
			string m_type;
			switch (pAdapter->Type)
			{
			case MIB_IF_TYPE_OTHER:
				m_type= "OTHER" ;
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_type= "ETHERNET";
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_type= "TOKENRING";
				break;
			case MIB_IF_TYPE_FDDI:
				m_type= "FDDI";
				break;
			case MIB_IF_TYPE_PPP:
				m_type= "PPP" ;
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_type= "LOOPBACK";
				break;
			case MIB_IF_TYPE_SLIP:
				m_type= "SLIP" ;
				break;
			default:
				break;
			}
			m_AdapterData.m_AdapterType=m_type;
			//cout << "网卡MAC地址 : ";
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
			m_AdapterData.m_macAddress=m_strMacAddress;
			m_vecAdapterData.push_back(m_AdapterData);
			pAdapter = pAdapter->Next;
			//cout << "--------------------------------------------------------------------" << endl;
		}
	}
	// 释放内存空间
	if (pIPAdapterInfo != NULL)
	{
		delete[] pIPAdapterInfo;
		pIPAdapterInfo = NULL;
	}
	//getchar();
	return m_vecAdapterData;
}

