#include <iostream>
#include "Rsa.h"
#include "Pem.h"
using namespace std;

int main(int argc, char **argv)
{
	// ԭʼ����    
	string srcText = "this is an example";

	string encryptText;
	string decryptText;

	// rsa
	//cout << "=== rsa�ӽ��� ===" << endl;
	//string key[2];
	//rsa_gen_key(key);

	cout << "��Կ: " << endl;
	cout << pub_key_pem << endl;
	cout << "˽Կ�� " << endl;
	cout << pri_key_pem << endl;
	encryptText = rsa_pub_encrypt(srcText, pub_key_pem);
	//cout << "�����ַ��� " << endl;
	//cout << encryptText << endl;
	decryptText = rsa_pri_decrypt(encryptText, pri_key_pem);
	cout << "�����ַ��� " << endl;
	cout << decryptText << endl;



	getchar();
	return 0;
}