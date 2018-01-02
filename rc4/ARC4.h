
#include <time.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
class RC4 {
public:
	/*
	构造函数，参数为密钥长度
	*/
	RC4(int kl) :keylen(kl) {
		srand((unsigned)time(NULL));
		for (int i = 0; i<kl; ++i){  //随机生产长度为keylen字节的密钥  
			int tmp = rand() % 256;
			K.push_back(char(tmp));
		}
	}
	/*
	由明文产生密文
	*/
	string encryption(const string &, const string &/*, const string &*/);
	/*
	解密类
	*/
	string decryption(string  keystream, string ciphertext);
private:
	unsigned char S[256]; //状态向量，共256字节  
	unsigned char T[256]; //临时向量，共256字节  
	int keylen;        //密钥长度，keylen个字节，取值范围为1-256  
	vector<char> K;      //可变长度密钥  
	vector<char> k;      //密钥流  
	string ciphertext, keystream;
	/*
	初始化状态向量S和临时向量T，供keyStream方法调用
	*/
	void initial() {
		for (int i = 0; i<256; ++i){
			S[i] = i;
			T[i] = K[i%keylen];
		}
	}
	/*
	初始排列状态向量S，供keyStream方法调用
	*/
	void rangeS() {
		int j = 0;
		for (int i = 0; i<256; ++i){
			j = (j + S[i] + T[i]) % 256;
			//cout<<"j="<<j<<endl;  
			S[i] = S[i] + S[j];
			S[j] = S[i] - S[j];
			S[i] = S[i] - S[j];
		}
	}
	/*
	生成密钥流
	len:明文为len个字节
	*/
	void keyStream(int len);
private:

};




