#pragma once
typedef unsigned int   uInt;  /* 16 bits or more */
typedef unsigned long  uLong; /* 32 bits or more */

class Gzip
{
public:
	/* 工厂方法 */
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
/* srcData 原数据 srcDataLen 原数据长度 zData 压缩后数据 zDataLen 压缩后长度 */
#define gzip_str   Gzip::Get()->Compress

/* Uncompress gzip data */
/* zData 数据 zDataLen 原数据长度 data 解压后数据 dataLen 解压后长度 */
#define ungzip_str Gzip::Get()->Uncompress

