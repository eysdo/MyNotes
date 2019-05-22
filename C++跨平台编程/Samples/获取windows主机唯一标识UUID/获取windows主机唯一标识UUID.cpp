#include <iostream>
#include "Uuid.h"
using namespace std;

int main(int argc, char **argv)
{

	cout << "UUID : " << Uuid::Get()->GetUUID() << endl;

	getchar();
	return 0;
}