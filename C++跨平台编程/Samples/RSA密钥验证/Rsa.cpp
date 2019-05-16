#include "RSA.h"
#include <openssl/rsa.h>    
#include <openssl/pem.h>
#include <cassert>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#ifdef WIN32
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif // WIN32


class CRSA : public Rsa
{
public:
	/* 生成RSA密钥对 */
	void GenerateRSAKey(std::string strKey[2])
	{
		// 公私密钥对    
		size_t pri_len;
		size_t pub_len;
		char *pri_key = NULL;
		char *pub_key = NULL;

		// 生成密钥对    
		RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

		BIO *pri = BIO_new(BIO_s_mem());
		BIO *pub = BIO_new(BIO_s_mem());

		PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
		PEM_write_bio_RSAPublicKey(pub, keypair);

		// 获取长度    
		pri_len = BIO_pending(pri);
		pub_len = BIO_pending(pub);

		// 密钥对读取到字符串    
		pri_key = (char *)malloc(pri_len + 1);
		pub_key = (char *)malloc(pub_len + 1);

		BIO_read(pri, pri_key, pri_len);
		BIO_read(pub, pub_key, pub_len);

		pri_key[pri_len] = '\0';
		pub_key[pub_len] = '\0';

		// 存储密钥对    
		strKey[0] = pub_key;
		strKey[1] = pri_key;

		// 存储到磁盘（这种方式存储的是begin rsa public key/ begin rsa private key开头的）  
		FILE *pubFile = fopen(PUB_KEY_FILE, "w");
		if (pubFile == NULL)
		{
			assert(false);
			return;
		}
		fputs(pub_key, pubFile);
		fclose(pubFile);

		FILE *priFile = fopen(PRI_KEY_FILE, "w");
		if (priFile == NULL)
		{
			assert(false);
			return;
		}
		fputs(pri_key, priFile);
		fclose(priFile);

		// 内存释放  
		RSA_free(keypair);
		BIO_free_all(pub);
		BIO_free_all(pri);

		free(pri_key);
		free(pub_key);
	}

	/* 公钥加密 */
	// 命令行方法生成公私钥对（begin public key/ begin private key）  
	// 找到openssl命令行工具，运行以下  
	// openssl genrsa -out prikey.pem 1024   
	// openssl rsa - in privkey.pem - pubout - out pubkey.pem
	std::string RsaPublicEncrypt(const std::string &clearText, const std::string &pubKey)
	{
		std::string strRet;
		RSA *rsa = NULL;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
		// 此处有三种方法  
		// 1, 读取内存里生成的密钥对，再从内存生成rsa  
		// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
		// 3，直接从读取文件指针生成rsa  
		RSA* pRSAPublicKey = RSA_new();
		rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);

		int len = RSA_size(rsa);
		char *encryptedText = (char *)malloc(len + 1);
		memset(encryptedText, 0, len + 1);

		// 加密函数  
		int ret = RSA_public_encrypt(clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
			strRet = std::string(encryptedText, ret);

		// 释放内存  
		free(encryptedText);
		BIO_free_all(keybio);
		RSA_free(rsa);

		return strRet;
	}

	/* 私钥解密 */
	std::string RsaPrivateDecrypt(const std::string &cipherText, const std::string &priKey)
	{
		std::string strRet;
		RSA *rsa = RSA_new();
		BIO *keybio;
		keybio = BIO_new_mem_buf((unsigned char *)priKey.c_str(), -1);

		// 此处有三种方法  
		// 1, 读取内存里生成的密钥对，再从内存生成rsa  
		// 2, 读取磁盘里生成的密钥对文本文件，在从内存生成rsa  
		// 3，直接从读取文件指针生成rsa  
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

		int len = RSA_size(rsa);
		char *decryptedText = (char *)malloc(len + 1);
		memset(decryptedText, 0, len + 1);

		// 解密函数  
		int ret = RSA_private_decrypt(cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
			strRet = std::string(decryptedText, ret);

		// 释放内存  
		free(decryptedText);
		BIO_free_all(keybio);
		RSA_free(rsa);

		return strRet;
	}

};

/* 工厂方法 */
Rsa* Rsa::Get()
{
	static Rsa* rsa = 0;
	if (0 == rsa)
	{
		static CRSA c_rsa;
		rsa = &c_rsa;
	}
	return rsa;
}

Rsa::Rsa()
{
}


Rsa::~Rsa()
{
}
