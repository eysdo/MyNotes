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

/* ѹ�� */
#define gzip_compress Gzip::Get()->Compress
/* ��ѹ�� */
#define gzip_uncompress Gzip::Get()->Uncompress

#endif // !GZIP_H



