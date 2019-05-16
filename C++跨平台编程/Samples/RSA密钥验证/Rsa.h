#pragma once
#include <string>

/* ---- rsa�ǶԳƼӽ��� ---- */    
#define KEY_LENGTH  2048             // ��Կ����  
#define PUB_KEY_FILE "pubkey.pem"    // ��Կ·��  
#define PRI_KEY_FILE "prikey.pem"    // ˽Կ·��

class Rsa
{
public:
	/* �������� */
	static Rsa* Get();

	/* ����RSA��Կ�� */  
	virtual void GenerateRSAKey(std::string strKey[2]) = 0;

	/* ��Կ���� */
	// �����з������ɹ�˽Կ�ԣ�begin public key/ begin private key��  
	// �ҵ�openssl�����й��ߣ���������  
	// openssl genrsa -out prikey.pem 1024   
	// openssl rsa - in privkey.pem - pubout - out pubkey.pem
	virtual std::string RsaPublicEncrypt(const std::string &clearText, const std::string &pubKey) = 0;

	/* ˽Կ���� */
	virtual std::string RsaPrivateDecrypt(const std::string &cipherText, const std::string &priKey) = 0;


public:
	Rsa();

protected:
	virtual ~Rsa();
};

/* ����RSA��Կ�� */
#define rsa_gen_key Rsa::Get()->GenerateRSAKey

/* ��Կ���� */
#define rsa_pub_encrypt Rsa::Get()->RsaPublicEncrypt

/* ˽Կ���� */
#define rsa_pri_decrypt Rsa::Get()->RsaPrivateDecrypt
