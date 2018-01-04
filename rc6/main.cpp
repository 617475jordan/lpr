#include <stdio.h>
#include <stdlib.h>
#include "rc6.h"
#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string ciphertext="htrhhy9hy4Äã¹þçêeuhgw5eththr4";
	string m_key = "rghregtgfby25t54rjhre89g254";
	string m_result;
	int lenFile = ciphertext.length();
	
	RC6 *m_rc6 = new RC6();
	cout << ciphertext << endl;
	m_result = m_rc6->encrypt(m_key, ciphertext, lenFile);
	cout << m_result << endl;
	ciphertext.clear();
	ciphertext = m_rc6->decrypt(m_key, m_result, lenFile);
	cout << ciphertext << endl;
	m_rc6 = NULL;
	delete[] m_rc6;
    return 0;
}

