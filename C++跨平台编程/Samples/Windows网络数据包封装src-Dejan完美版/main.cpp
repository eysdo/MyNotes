#include <iostream>
#include "Packet.h"
using namespace std;

enum MsgType
{
	ADMIN = 1, // 管理员
	USER   // 普通用户
};

// 发送信息 数据包
struct Msg :public  DjPacket::DataHeader
{
	// 初始化数据包头
	Msg() { _dataLenght = sizeof(Msg); }

	// 数据包体
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

	
	// 编码发送
	string send_data = DjPacket::serialize(&msg);

	// 编码接收
	string recv_data = DjPacket::unserialize(send_data);
	cout << "接收到数据的长度:" << send_data.length() << endl;
	cout << "原始数据的长度:" << recv_data.length() << endl;

	cout << "----------------------------------------" << endl;

	Msg*  recv = (Msg*)recv_data.c_str();
	cout << "用户ID:" << recv->id
		 << "\n姓名:" << recv->name
		 << "\n信息:" << recv->data
		 << "\n数据长度:" << recv->_dataLenght
		 << "\n数据类型:" << recv->_dataType << endl;


	getchar();
	return 0;
}