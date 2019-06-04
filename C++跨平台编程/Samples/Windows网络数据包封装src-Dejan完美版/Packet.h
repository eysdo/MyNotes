/*********************************************************************************
* Author: Dejan
* Version: v0.01
* Date: 2019.06.03
* Description: �������ݰ���װ
**********************************************************************************/

#ifndef PACKET_H
#define PACKET_H
#include <string>

namespace DjPacket {
	/* ���ݰ�ͷ���� */
	struct DataHeader
	{
		// ���ݰ�����
		int _dataType = 0;
		// ���ݰ���С
		unsigned long _dataLenght = 0;
	};

	class Packet
	{
	public:
		/* �������� */
		static Packet* Get();


		/** ���ݰ����л� & ���ݼ���
		* @param void* struct_pkt  �ṹ��
		* @return std::string
		*/
		virtual std::string Serialize(void* struct_pkt) = 0;

		/** ���ݰ������л� - ����&��ѹ����
		* @param  string& data  �ֽ�������
		* @return String
		*/
		virtual std::string Unserialize(std::string& data) = 0;


	protected:
		Packet() {}
	};

	/** ���ݰ����л� & ���ݼ���
	* @param void* struct_pkt  �ṹ��
	* @return std::string
	*/
	#define  serialize    Packet::Get()->Serialize

	/** ���ݰ������л� - ����&��ѹ����
	* @param  string& data  �ֽ�������
	* @return String
	*/
	#define  unserialize  Packet::Get()->Unserialize
}



#endif // !PACKET_H
