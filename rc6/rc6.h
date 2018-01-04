#pragma once
#include <string>
#include <stdint.h>
#define ROUNDS      20      // �ִ���
#define KEY_LENGTH  256     // ��Կ����
#define W           32      // �����ֳ���
using namespace     std;
// n = 128 λ�鳤�� (32+32+32+32)

// ������RC6
typedef struct rc6_ctx
{
	uint8_t r;      // ������Ĭ��20
	uint32_t *S;    // 32-�ҧڧ�ߧ�� ��ѧ�ߧէ�ӧ�� �ܧݧ���
} rc6_ctx_t;

class RC6
{
public:
	RC6();
	~RC6();
	//����
	string encrypt(string m_strKey, string ciphertext, int len);
	//����
	string decrypt(string m_strKey, string ciphertext, int len);
private:

	// ɾ��RC6������
	void ak_rc6_ctx_free(rc6_ctx_t *ctx);

	// ���ݧԧ��ڧ�� ��ѧ٧ӧק��ܧ� �ܧݧ���
	void ak_rc6_ctx_key_schedule(rc6_ctx_t *ctx, void *key);

	// �����㷨
	void ak_rc6_ctx_encrypt(rc6_ctx_t *ctx, void *block);

	// �����㷨
	void ak_rc6_ctx_decrypt(rc6_ctx_t *ctx, void *block);
protected:
	uint32_t rol32(uint32_t a, uint8_t n);
	uint32_t ror32(uint32_t a, uint8_t n);
};

class rc6Ctx
{
public:
	rc6Ctx();
	~rc6Ctx();
	rc6_ctx_t* ak_rc6_ctx_create_new();
	void ak_rc6_ctx_free(rc6_ctx_t *ctx);
};