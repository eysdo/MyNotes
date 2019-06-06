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
	/* ѹ�� */
	string Compress(string& data)
	{
		string gzip_data;
		// Gzipѹ��
		uLong data_len = data.length(); // ���ݰ� des���ܺ󳤶�
		uLong gzip_buf_size = sizeof(char) * compressBound(data_len + 1); // ����gzip��������С
		uLong gzip_dst_len = gzip_buf_size; // compress�����ݵĳ���

		char* gzip_buf = new char[gzip_buf_size](); // ����gzip������
		int err = compress((Byte*)gzip_buf, &gzip_dst_len, (const Byte*)data.c_str(), data_len);
		if (err != Z_OK)
		{
			delete[] gzip_buf;
			return "";
		}
		gzip_data.assign(gzip_buf, gzip_dst_len); // gzip_dst_len �Ѿ���ѹ����ĳ���
		gzip_data = to_string(data_len) + "," + gzip_data;  // ��ʽ: "des�������ݳ���,������"

		// �ռ��ͷ�
		delete[] gzip_buf;
		return gzip_data;
	}

	/* ��ѹ�� */
	string Uncompress(string& data)
	{
		char data_len[32] = { 0 }; // ���յ���gzip���ݳ���, buffer
		const Byte* pkt_data = (const Byte*)data.c_str(); // ��ʽ: "des�������ݳ���,������"
		int i = 0; // ����
		for (;; pkt_data++, ++i)
		{
			if (*pkt_data == ',') { ++pkt_data; break; }
			data_len[i] = *pkt_data;
		}
		uLong unz_data_len = strtoul(data_len, 0, 0) + 1; // �������gzip��ѹ����ĳ���
		char* unz_buf = new char[unz_data_len](); // ����gzip��ѹ��������
		uLong gzip_data_len = data.length() - (i + 1); // gzipѹ�����ݳ���
		int err = uncompress((Byte*)unz_buf, &unz_data_len, pkt_data, gzip_data_len);
		string unz_data;
		if (err != Z_OK)
		{
			delete[] unz_buf;
			return "";
		}
		unz_data.assign(unz_buf, unz_data_len);
		// �ռ��ͷ�
		delete[] unz_buf;
		return unz_data;
	}
};


/* �������� */
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

