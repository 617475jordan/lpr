
#include <time.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
class RC4 {
public:
	/*
	���캯��������Ϊ��Կ����
	*/
	RC4(int kl) :keylen(kl) {
		srand((unsigned)time(NULL));
		for (int i = 0; i<kl; ++i){  //�����������Ϊkeylen�ֽڵ���Կ  
			int tmp = rand() % 256;
			K.push_back(char(tmp));
		}
	}
	/*
	�����Ĳ�������
	*/
	string encryption(const string &, const string &/*, const string &*/);
	/*
	������
	*/
	string decryption(string  keystream, string ciphertext);
private:
	unsigned char S[256]; //״̬��������256�ֽ�  
	unsigned char T[256]; //��ʱ��������256�ֽ�  
	int keylen;        //��Կ���ȣ�keylen���ֽڣ�ȡֵ��ΧΪ1-256  
	vector<char> K;      //�ɱ䳤����Կ  
	vector<char> k;      //��Կ��  
	string ciphertext, keystream;
	/*
	��ʼ��״̬����S����ʱ����T����keyStream��������
	*/
	void initial() {
		for (int i = 0; i<256; ++i){
			S[i] = i;
			T[i] = K[i%keylen];
		}
	}
	/*
	��ʼ����״̬����S����keyStream��������
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
	������Կ��
	len:����Ϊlen���ֽ�
	*/
	void keyStream(int len);
private:

};




