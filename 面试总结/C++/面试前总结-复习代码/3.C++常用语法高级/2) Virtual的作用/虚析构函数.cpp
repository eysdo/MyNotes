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
	Animal() { cout << "Animal构造函数执行" << endl; }
	virtual ~Animal() { cout << "Animal析构函数执行" << endl; } // 不加上 virtual子类析构就调不到
};

class Dog :public Animal
{
public:
	void speak() { cout << "Dog speak()" << endl; }
	Dog() { cout << "Dog构造函数执行" << endl; }
	~Dog() { cout << "Dog析构函数执行" << endl; }
};

class Pig :public Animal
{
public:
	void speak() { cout << "Pig speak()" << endl; }
	Pig() { cout << "Pig构造函数执行" << endl; }
	~Pig() { cout << "Pig析构函数执行" << endl; }
};


int main(int argc, char **argv)
{
	Animal *dog = new Dog;
	delete dog;

	getchar();
	return 0;
}