#include "checkNetwork.h"
#include<iostream>  
#include <WINSOCK2.H>  
#pragma comment(lib,"ws2_32.lib")  
#define LEN 1024    //�������ݵĴ�С  
using namespace std;
checkNetwork::checkNetwork()
{
}


checkNetwork::~checkNetwork()
{
}

bool checkNetwork::networkState()
{
	//�����׽��ֿ�  
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);    //��ʼ��Socket��̬���ӿ�,����1.1�汾��winsocket��  

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||   //�ж������winsocket�ǲ���1.1�İ汾  
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();          //����  
		return false;                   //��ֹ��winsocketʹ��  
	}
	//WSADATA ws;  
	//WSAStartup(MAKEWORD(2,2),&ws);//  
	char http[3][60] = { "www.taobao.com", "www.google.com", "www.sina.com" };           //���ʹȸ���ҳ  
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//����socket  
	if (sock == INVALID_SOCKET)
	{
		//cout << "��������socket�׽���ʧ��!" << endl;
		//system("pause");
		return 0;
	}
	sockaddr_in hostadd;
	int m_success = 0;
	for (int m = 0; m < 3; m++)
	{
		hostent* host = gethostbyname(http[m]);//ȡ��������IP��ַ  
		if (host == NULL)
		{
			//cout << "��������û������״̬;" << endl;
	
			continue;
		}
		//cout << "������������״̬�����ڿ��Խ���ͨ��!" << endl;
		memcpy(&hostadd, host->h_addr, sizeof(hostadd));//�����ص�IP��ϢCopy����ַ�ṹ  
		hostadd.sin_family = AF_INET;
		hostadd.sin_port = htons(80);


		char buf[LEN] = "GET / HTTP/1.1\r\nHost: ";//����Http�������ݰ�  
		strcat(buf, inet_ntoa(hostadd.sin_addr));
		strcat(buf, " \r\nContent-Length: 10\r\n\r\n");
		strcat(buf, "Connection:close");


		int time = 1000;    //��ʱʱ��  
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&time, sizeof(time));


		if (connect(sock, (sockaddr*)&hostadd, sizeof(hostadd)) == SOCKET_ERROR)//��������  
		{
			//cout << "����ҳ��������ʧ��!" << endl;
			//system("pause");
			continue;
		}
		if (SOCKET_ERROR == send(sock, buf, strlen(buf) + 1, 0))//���͹���õ�Http�����  
		{
			//cout << "�������ݰ�ʧ��!" << endl;
			//system("pause");
			continue;
		}
		memset(buf, 0, LEN);
		recv(sock, buf, LEN, 0);               //���շ��ص�����  
		if (buf == NULL)
		{
			closesocket(sock);
			WSACleanup();
			host = NULL;
			delete[] host;
			continue;
		}
		
		host = NULL;
		delete[] host;
		m_success++;
		break;
	}
	if (m_success > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
