#include <windows.h>
#include <iostream>
#include <cstdio> 
using namespace std;

void main(){
    
    // system������ִ��ʱ������˳��ִ�н������Ĵ��룬����ͣ����system����ϣ�ֱ��cmdָ��ִ����ϣ�
    system("calc");

    // WinExec������ִ��ʱ����˳��ִ�н������Ĵ��룬������cmdָ���Ƿ�ִ����ɣ�
    WinExec("calc",SW_NORMAL);
}