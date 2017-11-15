#include "password.h"

#define random(x) (rand() % x) //通过取余取得指定范围的随机数
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
	////第1-4个字符
	//Box[0] = 87; //W
	//Box[1] = 79; //O
	//Box[2] = 89; //K
	//Box[3] = 69; //E
	m_vecOutput.push_back(87);
	m_vecOutput.push_back(79);
	m_vecOutput.push_back(89);
	m_vecOutput.push_back(69);
	//产生第5-8个字符

	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());

	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());
	m_vecOutput.push_back(RandomChar());

	//产生第13-16个字符

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


