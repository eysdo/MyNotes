#pragma once
#include <string>

class Des
{
public:
	/* 工厂方法 */
	static Des* Get();

	/* des对称加解密 - 加密 ecb模式 */
	virtual std::string DesEncrypt(const std::string &clearText, const std::string &key) = 0;

	/* des对称加解密 - 解密 ecb模式 */
	virtual std::string DesDecrypt(const std::string &cipherText, const std::string &key) = 0;



public:
	virtual ~Des();
protected:
	Des();
};

/* des对称加解密 - 加密 ecb模式 */
#define des_encrypt Des::Get()->DesEncrypt

/* des对称加解密 - 解密 ecb模式 */
#define des_decrypt Des::Get()->DesDecrypt

