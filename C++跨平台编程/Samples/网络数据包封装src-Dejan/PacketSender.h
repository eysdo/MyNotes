#ifndef PACKETSENDER_H
#define PACKETSENDER_H
#include <string>

/* ���ݰ�ͷ���� */
struct DataHeader
{
	// ���ݰ�����
	int _dataType = 0;
	// ���ݰ���С
	long long _dataLenght = 0;
	// ǩ��Token
	char _token[200] = { 0 };
};

class PacketSender
{
public:
	/* �������� */
	static PacketSender* Get();


	/** ���ݰ����л� & ���ݼ���
	 * @param void* struct_pkt  �ṹ��
	 * @return std::string
	 */
	virtual std::string Serialize(void* struct_pkt) = 0;

	/* ���ݰ������л� & ���ݽ���  */
	virtual std::string Unserialize(std::string& data) = 0;


protected:
	PacketSender() {}
};


#endif // !PACKETSENDER_H
