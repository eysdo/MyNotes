#ifndef GZIP_H
#define GZIP_H
#include <string>

class Gzip
{
public:
	/* 工厂方法 */
	static Gzip* Get();

	/* 压缩 */
	virtual std::string Compress(std::string& data) = 0;

	/* 解压缩 */
	virtual std::string Uncompress(std::string& data) = 0;


protected:
	Gzip() {}
};

/* 压缩 */
#define gzip_compress Gzip::Get()->Compress
/* 解压缩 */
#define gzip_uncompress Gzip::Get()->Uncompress

#endif // !GZIP_H



