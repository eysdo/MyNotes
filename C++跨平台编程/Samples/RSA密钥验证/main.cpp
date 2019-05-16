#include <iostream>
#include "base64.h"
#include "Rsa.h"
#include "Des.h"
#include "Pem.h"
using namespace std;

int main(int argc, char **argv)
{
	// ԭʼ����    
	string srcText = "this is an example";

	string encryptText;
	string decryptText;
	string base64_encryptText;

	// des    
	std::cout << "=== des�ӽ��� ===" << std::endl;
	std::string desKey = "12345";
	encryptText = des_encrypt(srcText, desKey);
	base64_encryptText = base64_encode(reinterpret_cast<const unsigned char*>(encryptText.c_str()), encryptText.length());
	std::cout << "�����ַ��� " << std::endl;
	std::cout << base64_encryptText << std::endl;
	decryptText = des_decrypt(encryptText, desKey);
	std::cout << "�����ַ��� " << std::endl;
	std::cout << decryptText << std::endl;

	// rsa
	//cout << "=== rsa�ӽ��� ===" << endl;
	//string key[2];
	//rsa_gen_key(key);

	cout << "��Կ: " << endl;
	cout << pub_key_pem << endl;
	cout << "˽Կ�� " << endl;
	cout << pri_key_pem << endl;
	encryptText = rsa_pub_encrypt(srcText, pub_key_pem);
	base64_encryptText = base64_encode(reinterpret_cast<const unsigned char*>(encryptText.c_str()), encryptText.length());
	cout << "�����ַ��� " << endl;
	cout << base64_encryptText << endl;

	decryptText = rsa_pri_decrypt(encryptText, pri_key_pem);
	decryptText = rsa_pri_decrypt(base64_decode(base64_encryptText), pri_key_pem);
	cout << "�����ַ��� " << endl;
	cout << decryptText << endl;

	/*
	RSA�ǶԳƼ������ݳ��������ƣ�1024λkey�����ֻ�ܼ���127λ���ݣ�����ͻᱨ��
	([color=red]javax.crypto.IllegalBlockSizeException: Data must not be longer 
	than 117 bytes[/color])����취���öԳƼ���(AES/DES etc)�������ݣ�Ȼ����RSA
	��Կ���ܶԳƼ��ܵ���Կ����RSA��˽Կ���ܵõ��ԳƼ��ܵ���Կ��Ȼ����ɷ������
	�õ����ġ�
	*/

	getchar();
	return 0;
}