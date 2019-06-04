/*********************************************************************************
* Author: Dejan
* Version: v0.01
* Date: 2019.06.03
* Description: 网络数据包封装
**********************************************************************************/

#ifndef PACKET_H
#define PACKET_H
#include <string>

namespace DjPacket {
	/* 数据包头类型 */
	struct DataHeader
	{
		// 数据包类型
		int _dataType = 0;
		// 数据包大小
		unsigned long _dataLenght = 0;
	};

	class Packet
	{
	public:
		/* 工厂方法 */
		static Packet* Get();


		/** 数据包序列化 & 数据加密
		* @param void* struct_pkt  结构体
		* @return std::string
		*/
		virtual std::string Serialize(void* struct_pkt) = 0;

		/** 数据包反序列化 - 解密&解压数据
		* @param  string& data  字节流数据
		* @return String
		*/
		virtual std::string Unserialize(std::string& data) = 0;


	protected:
		Packet() {}
	};

	/** 数据包序列化 & 数据加密
	* @param void* struct_pkt  结构体
	* @return std::string
	*/
	#define  serialize    Packet::Get()->Serialize

	/** 数据包反序列化 - 解密&解压数据
	* @param  string& data  字节流数据
	* @return String
	*/
	#define  unserialize  Packet::Get()->Unserialize
}



#endif // !PACKET_H
