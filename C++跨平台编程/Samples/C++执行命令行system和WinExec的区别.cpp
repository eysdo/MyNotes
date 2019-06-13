#include <windows.h>
#include <iostream>
#include <cstdio> 
using namespace std;

void main(){
    
    // system函数在执行时，不会顺序执行接下来的代码，而会停留在system语句上，直到cmd指令执行完毕；
    system("calc");

    // WinExec函数在执行时，会顺序执行接下来的代码，不管其cmd指令是否执行完成；
    WinExec("calc",SW_NORMAL);
}