#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal
{
public:
	virtual void speak()
	{
		cout << "Animal speak()" << endl;
	}
	Animal() { cout << "Animal���캯��ִ��" << endl; }
	virtual ~Animal() { cout << "Animal��������ִ��" << endl; } // ������ virtual���������͵�����
};

class Dog :public Animal
{
public:
	void speak() { cout << "Dog speak()" << endl; }
	Dog() { cout << "Dog���캯��ִ��" << endl; }
	~Dog() { cout << "Dog��������ִ��" << endl; }
};

class Pig :public Animal
{
public:
	void speak() { cout << "Pig speak()" << endl; }
	Pig() { cout << "Pig���캯��ִ��" << endl; }
	~Pig() { cout << "Pig��������ִ��" << endl; }
};


int main(int argc, char **argv)
{
	Animal *dog = new Dog;
	delete dog;

	getchar();
	return 0;
}