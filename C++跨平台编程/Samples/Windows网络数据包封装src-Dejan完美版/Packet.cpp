/*********************************************************************************
* Author: Dejan
* Version: v0.01
* Date: 2019.06.03
* Description: 网络数据包封装
**********************************************************************************/

#include "Packet.h"
#include "Des.h"
#ifdef WIN32
	#define ZLIB_WINAPI
#endif
#include <zlib/zlib.h>
#pragma comment(lib, "zlibstat.lib")

using namespace std;

#define DES_KEY "DGSDHRWJTYKsrtieuoqwunrne!@#$!#@2365323616idifkee!@@"

namespace DjPacket {
	class CPacket : public Packet
	{
	public:
		/** 数据包序列化  - 加密&压缩数据
		* @param void* struct_pkt  结构体
		* @return char*
		*/
		string Serialize(void* struct_pkt)
		{
			DataHeader* data_h = (DataHeader*)struct_pkt;
			string pkt_data;
			if (0 < data_h->_dataLenght)
				pkt_data.assign((char*)struct_pkt, data_h->_dataLenght);

			// Des编码加密
			pkt_data = des_encrypt(pkt_data, DES_KEY);
			// Gzip压缩
			uLong pkt_data_len = pkt_data.length(); // 数据包 des加密后长度
			uLong gzip_buf_size = sizeof(char) * compressBound(pkt_data_len + 1); // 计数gzip缓冲区大小
			uLong gzip_dst_len = gzip_buf_size; // compress后数据的长度
			
			char* gzip_buf = new char[gzip_buf_size](); // 创建gzip缓冲区
			int err = compress((Byte*)gzip_buf, &gzip_dst_len, (const Byte*)pkt_data.c_str(), pkt_data_len);
			if (err != Z_OK)
			{
				delete[] gzip_buf;
				return "";
			}
			pkt_data.assign(gzip_buf, gzip_dst_len); // gzip_dst_len 已经是压缩后的长度
			pkt_data = to_string(pkt_data_len) + "," + pkt_data;  // 格式: "des加密数据长度,数据流"

			// 空间释放
			delete[] gzip_buf;
			return pkt_data;
		}

		/** 数据包反序列化 - 解密&解压数据
		* @param  string& data  字节流数据
		* @return String
		*/
		string Unserialize(string& data)
		{
			char pkt_data_len[32] = { 0 }; // 接收到的gzip数据长度, buffer
			const Byte* pkt_data = (const Byte*)data.c_str(); // 格式: "des加密数据长度,数据流"
			int i = 0; // 索引
			for (;; pkt_data++, ++i)
			{
				if (*pkt_data == ',') { ++pkt_data; break; }
				pkt_data_len[i] = *pkt_data;
			}
			uLong unz_data_len = strtoul(pkt_data_len, 0, 0) + 1; // 获得数据gzip解压缩后的长度
			char* unz_buf = new char[unz_data_len](); // 创建gzip解压缩缓冲区
			uLong gzip_data_len = data.length() - (i + 1); // gzip压缩数据长度
			int err = uncompress((Byte*)unz_buf, &unz_data_len, pkt_data, gzip_data_len);
			string unz_data;
			if (err != Z_OK)
			{
				delete[] unz_buf;
				return "";
			}
			unz_data.assign(unz_buf, unz_data_len);
			// 空间释放
			delete[] unz_buf;
			return des_decrypt(unz_data, DES_KEY);
		}

	};

	/* 工厂方法 */
	Packet* Packet::Get()
	{
		static Packet* pktsd = 0;
		if (pktsd == 0)
		{
			static CPacket c_pktsd;
			pktsd = &c_pktsd;
		}
		return pktsd;
	}
}

