#include <iostream>
#include "Packet.h"
using namespace std;

enum MsgType
{
	ADMIN = 1, // ����Ա
	USER   // ��ͨ�û�
};

// ������Ϣ ���ݰ�
struct Msg :public  DjPacket::DataHeader
{
	// ��ʼ�����ݰ�ͷ
	Msg() { _dataLenght = sizeof(Msg); }

	// ���ݰ���
	int id = 0;
	char name[100] = { 0 };
	char data[1024] = { 0 };
};

int main(int argc, char **argv)
{
	Msg msg;
	msg.id = 123;
	strcpy(msg.name, "Dejan");
	strcpy(msg.data, "00000000000000000000000000000ffffffffffffffffffffffffffffff0-0-0-0-0-00000000000000000000000000000000000000-0-1");
	msg._dataType = USER;

	
	// ���뷢��
	string send_data = DjPacket::serialize(&msg);

	// �������
	string recv_data = DjPacket::unserialize(send_data);
	cout << "���յ����ݵĳ���:" << send_data.length() << endl;
	cout << "ԭʼ���ݵĳ���:" << recv_data.length() << endl;

	cout << "----------------------------------------" << endl;

	Msg*  recv = (Msg*)recv_data.c_str();
	cout << "�û�ID:" << recv->id
		 << "\n����:" << recv->name
		 << "\n��Ϣ:" << recv->data
		 << "\n���ݳ���:" << recv->_dataLenght
		 << "\n��������:" << recv->_dataType << endl;


	getchar();
	return 0;
}