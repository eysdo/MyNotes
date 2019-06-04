/*********************************************************************************
* Author: Dejan
* Version: v0.01
* Date: 2019.06.03
* Description: �������ݰ���װ
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
		/** ���ݰ����л�  - ����&ѹ������
		* @param void* struct_pkt  �ṹ��
		* @return char*
		*/
		string Serialize(void* struct_pkt)
		{
			DataHeader* data_h = (DataHeader*)struct_pkt;
			string pkt_data;
			if (0 < data_h->_dataLenght)
				pkt_data.assign((char*)struct_pkt, data_h->_dataLenght);

			// Des�������
			pkt_data = des_encrypt(pkt_data, DES_KEY);
			// Gzipѹ��
			uLong pkt_data_len = pkt_data.length(); // ���ݰ� des���ܺ󳤶�
			uLong gzip_buf_size = sizeof(char) * compressBound(pkt_data_len + 1); // ����gzip��������С
			uLong gzip_dst_len = gzip_buf_size; // compress�����ݵĳ���
			
			char* gzip_buf = new char[gzip_buf_size](); // ����gzip������
			int err = compress((Byte*)gzip_buf, &gzip_dst_len, (const Byte*)pkt_data.c_str(), pkt_data_len);
			if (err != Z_OK)
			{
				delete[] gzip_buf;
				return "";
			}
			pkt_data.assign(gzip_buf, gzip_dst_len); // gzip_dst_len �Ѿ���ѹ����ĳ���
			pkt_data = to_string(pkt_data_len) + "," + pkt_data;  // ��ʽ: "des�������ݳ���,������"

			// �ռ��ͷ�
			delete[] gzip_buf;
			return pkt_data;
		}

		/** ���ݰ������л� - ����&��ѹ����
		* @param  string& data  �ֽ�������
		* @return String
		*/
		string Unserialize(string& data)
		{
			char pkt_data_len[32] = { 0 }; // ���յ���gzip���ݳ���, buffer
			const Byte* pkt_data = (const Byte*)data.c_str(); // ��ʽ: "des�������ݳ���,������"
			int i = 0; // ����
			for (;; pkt_data++, ++i)
			{
				if (*pkt_data == ',') { ++pkt_data; break; }
				pkt_data_len[i] = *pkt_data;
			}
			uLong unz_data_len = strtoul(pkt_data_len, 0, 0) + 1; // �������gzip��ѹ����ĳ���
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
			return des_decrypt(unz_data, DES_KEY);
		}

	};

	/* �������� */
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

