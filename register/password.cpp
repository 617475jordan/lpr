#include "password.h"

#define random(x) (rand() % x) //ͨ��ȡ��ȡ��ָ����Χ�������
password::password()
{
}


password::~password()
{
}

std::vector<int> password::createPassword()
{
	std::vector<int> m_vecOutput;

	//int Box[16];
	////��1-4���ַ�
	//Box[0] = 87; //W
	//Box[1] = 79; //O
	//Box[2] = 89; //K
	//Box[3] = 69; //E
	m_vecOutput.push_back(87);
	m_vecOutput.push_back(79);
	m_vecOutput.push_back(89);
	m_vecOutput.push_back(69);
	//������5-8���ַ�

	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());

	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());

	//������13-16���ַ�

	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	return m_vecOutput;
}

bool password::CheckTheChar(int AsciiChar)
{
	if ((AsciiChar >= 48 && AsciiChar <= 57) || (AsciiChar >= 65 && AsciiChar <= 90))
		return true;
	else
		return false;
}

int password::RandomChar()
{
	int Num;
	do
	{
		Num = random(90);
	} while (CheckTheChar(Num) == false);
	return Num;
}


