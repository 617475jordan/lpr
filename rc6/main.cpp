#include <stdio.h>
#include <stdlib.h>
#include "rc6.h"

void test(unsigned char *key, unsigned char *txt)
{
	rc6Ctx *p = new rc6Ctx();
	rc6_ctx_t *m_p = p->ak_rc6_ctx_create_new();
    printf("Test vector:\n");
    printf("User key:\t");
    for(int i = 0; i < 32; ++i)
    {
        if (i == 16)
            printf("\n\t\t");
        if (key[i]<=9)
            printf("0");
        printf("%x ", key[i]);
    }
    printf("\n");

    printf("User text:\t");
    for(int i = 0; i < 16; ++i)
    {
        if (txt[i]<=9)
            printf("0");
        printf("%x ", txt[i]);
    }
    printf("\n");
	RC6 *m_rc6 = new RC6();
	m_rc6->ak_rc6_ctx_key_schedule(m_p, key);
	m_rc6->ak_rc6_ctx_encrypt(m_p, txt);
    printf("Cipher text:\t");
    for(int i = 0; i < 16; ++i)
    {
        if (txt[i]<=9)
            printf("0");
        printf("%x ", txt[i]);
    }
    printf("\n");

	m_rc6->ak_rc6_ctx_decrypt(m_p, txt);
    printf("User text:\t");
    for(int i = 0; i < 16; ++i)
    {
        if (txt[i]<=9)
            printf("0");
        printf("%x ", txt[i]);
    }
    printf("\n\n");
	m_rc6 = NULL;
	delete[] m_rc6;
	p->ak_rc6_ctx_free(m_p);
	m_p = NULL;
	delete[] m_p;
	p = NULL;
	delete[] p;
}


int main(void)
{
    unsigned char txt1[16] = {0};
    unsigned char key1[32] = {0};
    test(key1, txt1);

    unsigned char txt2[16] = {0x02,0x13,0x24,0x35,0x46,0x57,0x68,0x79,0x8A,0x9B,0xAC,0xBD,0xCE,0xDF,0xE0,0xF1};
    unsigned char key2[32] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x12,0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x9A,0xAB,0xBC,0xCD,0xDE,0xEF,0xF0,0x10,0x32,0x54,0x76,0x98,0xBA,0xDC,0xFE};
    test(key2, txt2);

    return 0;
}

