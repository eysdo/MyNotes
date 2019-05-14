#include "RSA.h"
#include <openssl/rsa.h>    
#include <openssl/pem.h>
#include <cassert>

#pragma comment(lib, "libcrypto.lib")

class CRSA : public Rsa
{
public:
	/* ����RSA��Կ�� */
	void GenerateRSAKey(std::string strKey[2])
	{
		// ��˽��Կ��    
		size_t pri_len;
		size_t pub_len;
		char *pri_key = NULL;
		char *pub_key = NULL;

		// ������Կ��    
		RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

		BIO *pri = BIO_new(BIO_s_mem());
		BIO *pub = BIO_new(BIO_s_mem());

		PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
		PEM_write_bio_RSAPublicKey(pub, keypair);

		// ��ȡ����    
		pri_len = BIO_pending(pri);
		pub_len = BIO_pending(pub);

		// ��Կ�Զ�ȡ���ַ���    
		pri_key = (char *)malloc(pri_len + 1);
		pub_key = (char *)malloc(pub_len + 1);

		BIO_read(pri, pri_key, pri_len);
		BIO_read(pub, pub_key, pub_len);

		pri_key[pri_len] = '\0';
		pub_key[pub_len] = '\0';

		// �洢��Կ��    
		strKey[0] = pub_key;
		strKey[1] = pri_key;

		// �洢�����̣����ַ�ʽ�洢����begin rsa public key/ begin rsa private key��ͷ�ģ�  
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

		// �ڴ��ͷ�  
		RSA_free(keypair);
		BIO_free_all(pub);
		BIO_free_all(pri);

		free(pri_key);
		free(pub_key);
	}

	/* ��Կ���� */
	// �����з������ɹ�˽Կ�ԣ�begin public key/ begin private key��  
	// �ҵ�openssl�����й��ߣ���������  
	// openssl genrsa -out prikey.pem 1024   
	// openssl rsa - in privkey.pem - pubout - out pubkey.pem
	std::string RsaPublicEncrypt(const std::string &clearText, const std::string &pubKey)
	{
		std::string strRet;
		RSA *rsa = NULL;
		BIO *keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
		// �˴������ַ���  
		// 1, ��ȡ�ڴ������ɵ���Կ�ԣ��ٴ��ڴ�����rsa  
		// 2, ��ȡ���������ɵ���Կ���ı��ļ����ڴ��ڴ�����rsa  
		// 3��ֱ�ӴӶ�ȡ�ļ�ָ������rsa  
		RSA* pRSAPublicKey = RSA_new();
		rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);

		int len = RSA_size(rsa);
		char *encryptedText = (char *)malloc(len + 1);
		memset(encryptedText, 0, len + 1);

		// ���ܺ���  
		int ret = RSA_public_encrypt(clearText.length(), (const unsigned char*)clearText.c_str(), (unsigned char*)encryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
			strRet = std::string(encryptedText, ret);

		// �ͷ��ڴ�  
		free(encryptedText);
		BIO_free_all(keybio);
		RSA_free(rsa);

		return strRet;
	}

	/* ˽Կ���� */
	std::string RsaPrivateDecrypt(const std::string &cipherText, const std::string &priKey)
	{
		std::string strRet;
		RSA *rsa = RSA_new();
		BIO *keybio;
		keybio = BIO_new_mem_buf((unsigned char *)priKey.c_str(), -1);

		// �˴������ַ���  
		// 1, ��ȡ�ڴ������ɵ���Կ�ԣ��ٴ��ڴ�����rsa  
		// 2, ��ȡ���������ɵ���Կ���ı��ļ����ڴ��ڴ�����rsa  
		// 3��ֱ�ӴӶ�ȡ�ļ�ָ������rsa  
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);

		int len = RSA_size(rsa);
		char *decryptedText = (char *)malloc(len + 1);
		memset(decryptedText, 0, len + 1);

		// ���ܺ���  
		int ret = RSA_private_decrypt(cipherText.length(), (const unsigned char*)cipherText.c_str(), (unsigned char*)decryptedText, rsa, RSA_PKCS1_PADDING);
		if (ret >= 0)
			strRet = std::string(decryptedText, ret);

		// �ͷ��ڴ�  
		free(decryptedText);
		BIO_free_all(keybio);
		RSA_free(rsa);

		return strRet;
	}

};

/* �������� */
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
