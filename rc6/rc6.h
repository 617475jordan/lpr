#pragma once
#include <string>
#include <stdint.h>
#define ROUNDS      20      // 轮次数
#define KEY_LENGTH  256     // 密钥长度
#define W           32      // 机器字长度
using namespace     std;
// n = 128 位块长度 (32+32+32+32)

// 上下文RC6
typedef struct rc6_ctx
{
	uint8_t r;      // 轮数，默认20
	uint32_t *S;    // 32-битные раундовые ключи
} rc6_ctx_t;

class RC6
{
public:
	RC6();
	~RC6();
	//加密
	string encrypt(string m_strKey, string ciphertext, int len);
	//解密
	string decrypt(string m_strKey, string ciphertext, int len);
private:

	// 删除RC6上下文
	void ak_rc6_ctx_free(rc6_ctx_t *ctx);

	// Алгоритм развёртки ключа
	void ak_rc6_ctx_key_schedule(rc6_ctx_t *ctx, void *key);

	// 加密算法
	void ak_rc6_ctx_encrypt(rc6_ctx_t *ctx, void *block);

	// 解密算法
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