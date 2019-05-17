#pragma once
typedef unsigned int   uInt;  /* 16 bits or more */
typedef unsigned long  uLong; /* 32 bits or more */

class Gzip
{
public:
	/* �������� */
	static Gzip* Get();

	/* gzCompress: do the compressing */
	virtual int Compress(const char *src, uLong srcLen, char *dest, uLong destLen) = 0;

	/* gzDecompress: do the decompressing */
	virtual int Uncompress(const char *src, uLong srcLen, const char *dst, uLong dstLen) = 0;



public:
	virtual ~Gzip();

protected:
	Gzip();
};

/* Compress gzip data */
/* srcData ԭ���� srcDataLen ԭ���ݳ��� zData ѹ�������� zDataLen ѹ���󳤶� */
#define gzip_str   Gzip::Get()->Compress

/* Uncompress gzip data */
/* zData ���� zDataLen ԭ���ݳ��� data ��ѹ������ dataLen ��ѹ�󳤶� */
#define ungzip_str Gzip::Get()->Uncompress

