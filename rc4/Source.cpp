#include "ARC4.h"
#include <string>
using namespace std;
int main()
{
	RC4 *m_rc4 = new RC4(128); //��Կ��128�ֽ�  
	string m_inpassword,m_outpassword,m_password;
	m_password = "����.txtr5yh5te4hu54h";
	m_inpassword = m_rc4->encryption(m_password, "��Կ��.txt");
	m_outpassword = m_rc4->decryption("��Կ��.txt", m_inpassword);
	cout << m_outpassword << endl;
	m_rc4 = NULL;
	delete[] m_rc4;

	return 0;
}