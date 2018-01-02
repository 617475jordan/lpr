#include "ARC4.h"
#include <string>
using namespace std;
int main()
{
	RC4 *m_rc4 = new RC4(128); //密钥长128字节  
	string m_inpassword,m_outpassword,m_password;
	m_password = "明文.txtr5yh5te4hu54h";
	m_inpassword = m_rc4->encryption(m_password, "密钥流.txt");
	m_outpassword = m_rc4->decryption("密钥流.txt", m_inpassword);
	cout << m_outpassword << endl;
	m_rc4 = NULL;
	delete[] m_rc4;

	return 0;
}