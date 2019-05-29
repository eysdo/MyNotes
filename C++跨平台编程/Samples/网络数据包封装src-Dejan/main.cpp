#include <iostream>
#include "PacketSender.h"
using namespace std;


enum MsgType
{
	ADMIN = 1, // ����Ա
	USER   // ��ͨ�û�
};

// ������Ϣ ���ݰ�
struct Msg :public DataHeader
{
	// ��ʼ�����ݰ�ͷ
	Msg() { _dataLenght = sizeof(Msg); }

	// ���ݰ���
	int id = 0;
	char name[100] = { 0 };
};

int main(int argc, char **argv)
{
	Msg msg;
	msg.id = 123;
	strcpy(msg.name, "Dejan");
	msg._dataType = USER;
	
	// struct > char* > des 
	string send_data = PacketSender::Get()->Serialize(&msg);

	// �������
	string recv_data = PacketSender::Get()->Unserialize(send_data);
	cout << "�������ݵĳ���:" << send_data.length() << endl;
	cout << "�������ݵĳ���:" << recv_data.length() << endl;

	Msg* msg2 = (Msg*)recv_data.c_str();
	cout <<"ID:"<< msg2->id<<"\nname:"<<msg2->name<<
		"\n���ݳ���:"<<msg2->_dataLenght<<"\n��������:"<<msg2->_dataType << endl;



	getchar();
	return 0;
}