#include <iostream>
#include "Rsa.h"
#include "Pem.h"
using namespace std;

int main(int argc, char **argv)
{
	// 原始明文    
	string srcText = "this is an example";

	string encryptText;
	string decryptText;

	// rsa
	//cout << "=== rsa加解密 ===" << endl;
	//string key[2];
	//rsa_gen_key(key);

	cout << "公钥: " << endl;
	cout << pub_key_pem << endl;
	cout << "私钥： " << endl;
	cout << pri_key_pem << endl;
	encryptText = rsa_pub_encrypt(srcText, pub_key_pem);
	//cout << "加密字符： " << endl;
	//cout << encryptText << endl;
	decryptText = rsa_pri_decrypt(encryptText, pri_key_pem);
	cout << "解密字符： " << endl;
	cout << decryptText << endl;



	getchar();
	return 0;
}