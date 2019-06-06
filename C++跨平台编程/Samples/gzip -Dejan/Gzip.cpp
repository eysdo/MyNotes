#include "Gzip.h"
using namespace std;
#ifdef WIN32
#define ZLIB_WINAPI
#endif
#include <zlib/zlib.h>
#pragma comment(lib, "zlibstat.lib")

class CGzip : public Gzip
{
public:
	/* 压缩 */
	string Compress(string& data)
	{
		string gzip_data;
		// Gzip压缩
		uLong data_len = data.length(); // 数据包 des加密后长度
		uLong gzip_buf_size = sizeof(char) * compressBound(data_len + 1); // 计数gzip缓冲区大小
		uLong gzip_dst_len = gzip_buf_size; // compress后数据的长度

		char* gzip_buf = new char[gzip_buf_size](); // 创建gzip缓冲区
		int err = compress((Byte*)gzip_buf, &gzip_dst_len, (const Byte*)data.c_str(), data_len);
		if (err != Z_OK)
		{
			delete[] gzip_buf;
			return "";
		}
		gzip_data.assign(gzip_buf, gzip_dst_len); // gzip_dst_len 已经是压缩后的长度
		gzip_data = to_string(data_len) + "," + gzip_data;  // 格式: "des加密数据长度,数据流"

		// 空间释放
		delete[] gzip_buf;
		return gzip_data;
	}

	/* 解压缩 */
	string Uncompress(string& data)
	{
		char data_len[32] = { 0 }; // 接收到的gzip数据长度, buffer
		const Byte* pkt_data = (const Byte*)data.c_str(); // 格式: "des加密数据长度,数据流"
		int i = 0; // 索引
		for (;; pkt_data++, ++i)
		{
			if (*pkt_data == ',') { ++pkt_data; break; }
			data_len[i] = *pkt_data;
		}
		uLong unz_data_len = strtoul(data_len, 0, 0) + 1; // 获得数据gzip解压缩后的长度
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
		return unz_data;
	}
};


/* 工厂方法 */
Gzip* Gzip::Get()
{
	static Gzip* gz = 0;
	if (gz == 0)
	{
		static CGzip c_gz;
		gz = &c_gz;
	}
	return gz;
}

