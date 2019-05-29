#include "PacketSender.h"
#include "Des.h"
#include "Gzip.h"
using namespace std;

#define DES_KEY "DGSDHRWJTYKsrtieuoqwunrne!@#$!#@2365323616idifkee!@@"
#define GZIP_BUF_MAX_SIZE  4194304  // 4 MB ��С

class CPacketSender : public PacketSender
{
public:
	/** ���ݰ����л� 
	 * @param void* struct_pkt  �ṹ��
	 * @return char*
	 */
	string Serialize(void* struct_pkt)
	{
		DataHeader* data_h = (DataHeader*)struct_pkt;
		string data_buf;
		if(0 < data_h->_dataLenght)
			data_buf.assign((char*)struct_pkt, data_h->_dataLenght);

		// Des�������
		data_buf = des_encrypt(data_buf, DES_KEY);
		// Gzipѹ��
		uLong gzip_buf_size = data_buf.length();
		char* gzip_buf = new char[gzip_buf_size + 1]();
		uLong ret = gzip_str(data_buf.c_str(), gzip_buf_size, gzip_buf, gzip_buf_size + 1);
		if (0 >= ret) data_buf.clear();
		data_buf.assign(gzip_buf, ret);
		// �ռ��ͷ�
		delete[] gzip_buf;
		return data_buf;
	}

	/* ���ݰ������л� & ���ݽ���  */
	std::string Unserialize(string& data)
	{
		uLong unz_len = data.length(); // ���ݵĳ���
		char* unz_buf = new char[GZIP_BUF_MAX_SIZE]();
		ungzip_str(data.c_str(), unz_len, unz_buf, GZIP_BUF_MAX_SIZE);
		string unz_data(unz_buf);
		// �ռ��ͷ�
		delete[] unz_buf;
		return des_decrypt(unz_data, DES_KEY);
	}

};

/* �������� */
PacketSender* PacketSender::Get()
{
	static PacketSender* pktsd = 0;
	if (pktsd == 0)
	{
		static CPacketSender c_pktsd;
		pktsd = &c_pktsd;
	}
	return pktsd;
}