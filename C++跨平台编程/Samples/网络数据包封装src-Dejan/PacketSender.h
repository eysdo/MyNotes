#ifndef PACKETSENDER_H
#define PACKETSENDER_H
#include <string>

/* 数据包头类型 */
struct DataHeader
{
	// 数据包类型
	int _dataType = 0;
	// 数据包大小
	long long _dataLenght = 0;
	// 签名Token
	char _token[200] = { 0 };
};

class PacketSender
{
public:
	/* 工厂方法 */
	static PacketSender* Get();


	/** 数据包序列化 & 数据加密
	 * @param void* struct_pkt  结构体
	 * @return std::string
	 */
	virtual std::string Serialize(void* struct_pkt) = 0;

	/* 数据包反序列化 & 数据解密  */
	virtual std::string Unserialize(std::string& data) = 0;


protected:
	PacketSender() {}
};


#endif // !PACKETSENDER_H
