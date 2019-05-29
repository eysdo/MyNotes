#include "Des.h"
#include <openssl/des.h>
#include <vector>
using namespace std;

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Advapi32.lib")
#ifdef WIN32
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif // WIN32

class CDes : public Des
{
public:
	/* des�ԳƼӽ��� - ���� ecbģʽ */
	string DesEncrypt(const string &clearText, const string &key)
	{
		std::string cipherText; // ����    

		DES_cblock keyEncrypt;
		memset(keyEncrypt, 0, 8);

		// ���첹������Կ    
		if (key.length() <= 8)
			memcpy(keyEncrypt, key.c_str(), key.length());
		else
			memcpy(keyEncrypt, key.c_str(), 8);

		// ��Կ�û�    
		DES_key_schedule keySchedule;
		DES_set_key_unchecked(&keyEncrypt, &keySchedule);

		// ѭ�����ܣ�ÿ8�ֽ�һ��    
		const_DES_cblock inputText;
		DES_cblock outputText;
		std::vector<unsigned char> vecCiphertext;
		unsigned char tmp[8];

		for (int i = 0; i < clearText.length() / 8; i++)
		{
			memcpy(inputText, clearText.c_str() + i * 8, 8);
			DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
			memcpy(tmp, outputText, 8);

			for (int j = 0; j < 8; j++)
				vecCiphertext.push_back(tmp[j]);
		}

		if (clearText.length() % 8 != 0)
		{
			int tmp1 = clearText.length() / 8 * 8;
			int tmp2 = clearText.length() - tmp1;
			memset(inputText, 0, 8);
			memcpy(inputText, clearText.c_str() + tmp1, tmp2);
			// ���ܺ���    
			DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_ENCRYPT);
			memcpy(tmp, outputText, 8);

			for (int j = 0; j < 8; j++)
				vecCiphertext.push_back(tmp[j]);
		}

		cipherText.clear();
		cipherText.assign(vecCiphertext.begin(), vecCiphertext.end());

		return cipherText;
	}

	/* des�ԳƼӽ��� - ���� ecbģʽ */
	string DesDecrypt(const string &cipherText, const string &key)
	{
		std::string clearText; // ����    

		DES_cblock keyEncrypt;
		memset(keyEncrypt, 0, 8);

		if (key.length() <= 8)
			memcpy(keyEncrypt, key.c_str(), key.length());
		else
			memcpy(keyEncrypt, key.c_str(), 8);

		DES_key_schedule keySchedule;
		DES_set_key_unchecked(&keyEncrypt, &keySchedule);

		const_DES_cblock inputText;
		DES_cblock outputText;
		std::vector<unsigned char> vecCleartext;
		unsigned char tmp[8];

		for (int i = 0; i < cipherText.length() / 8; i++)
		{
			memcpy(inputText, cipherText.c_str() + i * 8, 8);
			DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
			memcpy(tmp, outputText, 8);

			for (int j = 0; j < 8; j++)
				vecCleartext.push_back(tmp[j]);
		}

		if (cipherText.length() % 8 != 0)
		{
			int tmp1 = cipherText.length() / 8 * 8;
			int tmp2 = cipherText.length() - tmp1;
			memset(inputText, 0, 8);
			memcpy(inputText, cipherText.c_str() + tmp1, tmp2);
			// ���ܺ���    
			DES_ecb_encrypt(&inputText, &outputText, &keySchedule, DES_DECRYPT);
			memcpy(tmp, outputText, 8);

			for (int j = 0; j < 8; j++)
				vecCleartext.push_back(tmp[j]);
		}

		clearText.clear();
		clearText.assign(vecCleartext.begin(), vecCleartext.end());

		return clearText;
	}

};

/* �������� */
Des* Des::Get()
{
	static Des* des = 0;
	if (0 == des)
	{
		static CDes c_des;
		des = &c_des;
	}
	return des;
}


Des::Des()
{
}


Des::~Des()
{
}
