#include "ARC4.h"


void RC4::keyStream(int len) {
	initial();
	rangeS();

	int i = 0, j = 0, t;
	while (len--){
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;

		S[i] = S[i] + S[j];
		S[j] = S[i] - S[j];
		S[i] = S[i] - S[j];

		t = (S[i] + S[j]) % 256;
		k.push_back(S[t]);
	}
}
string RC4::encryption(const string &plaintext, const string &ks/*, const string &ciphertext*/)
{
	ofstream  outks;
	string ciphertext;

	int lenFile=plaintext.length();
	//生产密钥流  
	keyStream(lenFile);
	outks.open(ks);
	for (int i = 0; i < lenFile; ++i){
		outks << (k[i]);
	}
	outks.close();


	unsigned char *bits = new unsigned char[lenFile];
	//将明文按字节依次与密钥流异或后输出到密文文件中  
	for (int i = 0; i < lenFile; ++i)
	{
		bits[i] = (unsigned char)(plaintext[i] ^ k[i]);
	}
	ciphertext = (char*)bits;
	bits = NULL;
	delete[] bits;
	ciphertext;
	return ciphertext;
}

string RC4::decryption(string  keystream, string ciphertext)
{
	ifstream inks;
	//ofstream out;

	inks.open(keystream);
	//incp.open(ciphertext);

	//计算密文长度  
	inks.seekg(0, ios::end);
	const int lenFile = inks.tellg();
	inks.seekg(0, ios::beg);
	//读入密钥流  
	unsigned char *bitKey = new unsigned char[lenFile];
	inks.read((char *)bitKey, lenFile);
	inks.close();
	//读入密文  
	unsigned char *bits = new unsigned char[lenFile];
	//unsigned char *bitCip = new unsigned char[lenFile];
	//解密后结果输出到解密文件  
	for (int i = 0; i < lenFile; ++i)
	{
		//bitCip[i] = ciphertext[i];
		bits[i] = (unsigned char)(ciphertext[i] ^ bitKey[i]);

	}

	string m_ciphertext = (char*)bits;
	bits = NULL;
	delete[] bits;
	return m_ciphertext;

}
