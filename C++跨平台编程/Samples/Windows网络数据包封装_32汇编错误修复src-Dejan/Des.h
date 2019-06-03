#pragma once
#include <string>

class Des
{
public:
	/* �������� */
	static Des* Get();

	/* des�ԳƼӽ��� - ���� ecbģʽ */
	virtual std::string DesEncrypt(const std::string &clearText, const std::string &key) = 0;

	/* des�ԳƼӽ��� - ���� ecbģʽ */
	virtual std::string DesDecrypt(const std::string &cipherText, const std::string &key) = 0;



public:
	virtual ~Des();
protected:
	Des();
};

/* des�ԳƼӽ��� - ���� ecbģʽ */
#define des_encrypt Des::Get()->DesEncrypt

/* des�ԳƼӽ��� - ���� ecbģʽ */
#define des_decrypt Des::Get()->DesDecrypt

