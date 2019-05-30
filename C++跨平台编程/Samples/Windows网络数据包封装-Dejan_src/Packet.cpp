#include "Packet.h"
#include "Des.h"
#include "Gzip.h"
using namespace std;

#define DES_KEY "DGSDHRWJTYKsrtieuoqwunrne!@#$!#@2365323616idifkee!@@"
#define GZIP_BUF_MAX_SIZE  4194304  // 4 MB 大小

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
			string data_buf;
			if (0 < data_h->_dataLenght)
				data_buf.assign((char*)struct_pkt, data_h->_dataLenght);

			// Des编码加密
			data_buf = des_encrypt(data_buf, DES_KEY);
			// Gzip压缩
			uLong gzip_buf_size = data_buf.length();
			char* gzip_buf = new char[gzip_buf_size + 1]();
			uLong ret = gzip_str(data_buf.c_str(), gzip_buf_size, gzip_buf, gzip_buf_size + 1);
			if (0 >= ret) data_buf.clear();
			data_buf.assign(gzip_buf, ret);
			// 空间释放
			delete[] gzip_buf;
			return data_buf;
		}

		/** 数据包反序列化 - 解密&解压数据
		* @param  string& data  字节流数据
		* @return String
		*/
		string Unserialize(string& data)
		{
			uLong unz_len = data.length(); // 数据的长度
			char* unz_buf = new char[GZIP_BUF_MAX_SIZE]();
			ungzip_str(data.c_str(), unz_len, unz_buf, GZIP_BUF_MAX_SIZE);
			string unz_data(unz_buf);
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

