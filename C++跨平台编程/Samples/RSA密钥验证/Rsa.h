#pragma once
#include <string>

/* ---- rsa非对称加解密 ---- */    
#define KEY_LENGTH  2048             // 密钥长度  
#define PUB_KEY_FILE "pubkey.pem"    // 公钥路径  
#define PRI_KEY_FILE "prikey.pem"    // 私钥路径

class Rsa
{
public:
	/* 工厂方法 */
	static Rsa* Get();

	/* 生成RSA密钥对 */  
	virtual void GenerateRSAKey(std::string strKey[2]) = 0;

	/* 公钥加密 */
	// 命令行方法生成公私钥对（begin public key/ begin private key）  
	// 找到openssl命令行工具，运行以下  
	// openssl genrsa -out prikey.pem 1024   
	// openssl rsa - in privkey.pem - pubout - out pubkey.pem
	virtual std::string RsaPublicEncrypt(const std::string &clearText, const std::string &pubKey) = 0;

	/* 私钥解密 */
	virtual std::string RsaPrivateDecrypt(const std::string &cipherText, const std::string &priKey) = 0;


public:
	Rsa();

protected:
	virtual ~Rsa();
};

/* 生成RSA密钥对 */
#define rsa_gen_key Rsa::Get()->GenerateRSAKey

/* 公钥加密 */
#define rsa_pub_encrypt Rsa::Get()->RsaPublicEncrypt

/* 私钥解密 */
#define rsa_pri_decrypt Rsa::Get()->RsaPrivateDecrypt
