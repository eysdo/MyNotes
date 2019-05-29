#include <iostream>
#include "PacketSender.h"
using namespace std;


enum MsgType
{
	ADMIN = 1, // 管理员
	USER   // 普通用户
};

// 发送信息 数据包
struct Msg :public DataHeader
{
	// 初始化数据包头
	Msg() { _dataLenght = sizeof(Msg); }

	// 数据包体
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

	// 编码接收
	string recv_data = PacketSender::Get()->Unserialize(send_data);
	cout << "发送数据的长度:" << send_data.length() << endl;
	cout << "接收数据的长度:" << recv_data.length() << endl;

	Msg* msg2 = (Msg*)recv_data.c_str();
	cout <<"ID:"<< msg2->id<<"\nname:"<<msg2->name<<
		"\n数据长度:"<<msg2->_dataLenght<<"\n数据类型:"<<msg2->_dataType << endl;



	getchar();
	return 0;
}