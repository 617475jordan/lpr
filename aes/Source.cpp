#include <iostream>  
#include "AES.h"  
 
using namespace std;


int main(int argc, char **argv)
{
	AES *m_aes = new AES();
	string str1 = "qwertyuisfdlsajdxcvnkhsakfh1332487";
	cout << "加密前:" << str1 << endl;
	string str2 = m_aes->EncryptionAES(str1);
	cout << "加密后:" << str2 << endl;
	string str3 = m_aes->DecryptionAES(str2);
	cout << "解密后:" << str3 << endl;
	delete[] m_aes;
	system("pause");
	return 0;
}