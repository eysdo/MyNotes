#include <iostream>
#include "base64.h"
#include "Rsa.h"
#include "Des.h"
#include "Pem.h"
using namespace std;

int main(int argc, char **argv)
{
	// 原始明文    
	string srcText = "this is an example";

	string encryptText;
	string decryptText;
	string base64_encryptText;

	// des    
	std::cout << "=== des加解密 ===" << std::endl;
	std::string desKey = "12345";
	encryptText = des_encrypt(srcText, desKey);
	base64_encryptText = base64_encode(reinterpret_cast<const unsigned char*>(encryptText.c_str()), encryptText.length());
	std::cout << "加密字符： " << std::endl;
	std::cout << base64_encryptText << std::endl;
	decryptText = des_decrypt(encryptText, desKey);
	std::cout << "解密字符： " << std::endl;
	std::cout << decryptText << std::endl;

	// rsa
	//cout << "=== rsa加解密 ===" << endl;
	//string key[2];
	//rsa_gen_key(key);

	cout << "公钥: " << endl;
	cout << pub_key_pem << endl;
	cout << "私钥： " << endl;
	cout << pri_key_pem << endl;
	encryptText = rsa_pub_encrypt(srcText, pub_key_pem);
	base64_encryptText = base64_encode(reinterpret_cast<const unsigned char*>(encryptText.c_str()), encryptText.length());
	cout << "加密字符： " << endl;
	cout << base64_encryptText << endl;

	decryptText = rsa_pri_decrypt(encryptText, pri_key_pem);
	decryptText = rsa_pri_decrypt(base64_decode(base64_encryptText), pri_key_pem);
	cout << "解密字符： " << endl;
	cout << decryptText << endl;

	/*
	RSA非对称加密内容长度有限制，1024位key的最多只能加密127位数据，否则就会报错
	([color=red]javax.crypto.IllegalBlockSizeException: Data must not be longer 
	than 117 bytes[/color])解决办法是用对称加密(AES/DES etc)加密数据，然后用RSA
	公钥加密对称加密的密钥，用RSA的私钥解密得到对称加密的密钥，然后完成反向操作
	得到明文。
	*/

	getchar();
	return 0;
}