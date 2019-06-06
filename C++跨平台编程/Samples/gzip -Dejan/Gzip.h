#ifndef GZIP_H
#define GZIP_H
#include <string>

class Gzip
{
public:
	/* �������� */
	static Gzip* Get();

	/* ѹ�� */
	virtual std::string Compress(std::string& data) = 0;

	/* ��ѹ�� */
	virtual std::string Uncompress(std::string& data) = 0;


protected:
	Gzip() {}
};

#endif // !GZIP_H



