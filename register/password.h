#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class password
{
public:
	password();
	~password();
	std::vector<int>  createPassword();
private:
	bool CheckTheChar(int AsciiChar);
	int RandomChar();
};

