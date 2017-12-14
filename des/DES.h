#pragma once
#include "memory.h"  
#include "stdio.h"  
#include <iostream>
using namespace std;
enum
{
	encrypt, decrypt
};//ENCRYPT:加密，DECRYPT：解密  
struct desdata
{
	char    m_crypt[128];
	int     m_len;
	int     m_passwordNum;
	bool    m_type;
};

class DES
{
public:
	DES();
	~DES();
	//desdata                   getCrypt(desdata m_desData);
	void                      running(char in[128], char out[128], int num, bool type);

private:
	void                      processing(char out[8], char in[8], bool type);
	//设置密钥 
	void                      initial(int num);
protected:
	static void               f_func(bool in[32], const bool ki[48]);//f函数  
	static void               s_func(bool out[32], const bool in[48]);//s盒代替  
	//变换  
	static void               transform(bool *out, bool *in, const char *table, int len);
	static void               xor(bool *ina, const bool *inb, int len);//异或  
	static void               rotatel(bool *in, int len, int loop);//循环左移  
	//字节组转换成位组  
	static void               bytetobit(bool *out, const char *in, int bits);
	//位组转换成字节组     
	static void               bittobyte(char *out, const bool *in, int bits);
};

