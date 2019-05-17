#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib/zlib.h>

// gzCompress: do the compressing
int gzCompress(const char *src, int srcLen, char *dest, int destLen)
{
	z_stream c_stream;
	int err = 0;
	int windowBits = 15;
	int GZIP_ENCODING = 16;

	if (src && srcLen > 0)
	{
		c_stream.zalloc = (alloc_func)0;
		c_stream.zfree = (free_func)0;
		c_stream.opaque = (voidpf)0;
		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
			windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
		c_stream.next_in = (Bytef *)src;
		c_stream.avail_in = srcLen;
		c_stream.next_out = (Bytef *)dest;
		c_stream.avail_out = destLen;
		while (c_stream.avail_in != 0 && c_stream.total_out < destLen)
		{
			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
		}
		if (c_stream.avail_in != 0) return c_stream.avail_in;
		for (;;) {
			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
			if (err != Z_OK) return -1;
		}
		if (deflateEnd(&c_stream) != Z_OK) return -1;
		return c_stream.total_out;
	}
	return -1;
}

// gzDecompress: do the decompressing
int gzDecompress(const char *src, int srcLen, const char *dst, int dstLen) {
	z_stream strm;
	strm.zalloc = NULL;
	strm.zfree = NULL;
	strm.opaque = NULL;

	strm.avail_in = srcLen;
	strm.avail_out = dstLen;
	strm.next_in = (Bytef *)src;
	strm.next_out = (Bytef *)dst;

	int err = -1, ret = -1;
	err = inflateInit2(&strm, MAX_WBITS + 16);
	if (err == Z_OK) {
		err = inflate(&strm, Z_FINISH);
		if (err == Z_STREAM_END) {
			ret = strm.total_out;
		}
		else {
			inflateEnd(&strm);
			return err;
		}
	}
	else {
		inflateEnd(&strm);
		return err;
	}
	inflateEnd(&strm);
	return err;
}

int main()
{
	char* src = "just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk" \
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk"\
		"just for test, dd, dd, dd.....hkjhkjhkjhkjhkhkhkhhjkhkhjhkhkjhkjhkjhkjhjkhkjhjkhjk";
	int size_src = strlen(src);
	char* compressed = (char*)malloc(size_src * 2);
	memset(compressed, 0, size_src * 2);
	printf("to compress src: %s\n", src);
	printf("to compress src size: %d\n", size_src);

	int gzSize = gzCompress(src, size_src, compressed, size_src * 2);
	if (gzSize <= 0)
	{
		printf("compress error.\n");
		return -1;
	}
	printf("compressed: ");
	int i = 0;
	for (; i<gzSize; ++i)
	{
		printf("%02x ", compressed[i]);
	}
	printf("\ncompressed size: %d\n", gzSize);

	char* uncompressed = (char*)malloc(size_src * 2);
	memset(uncompressed, 0, size_src * 2);
	int ret = gzDecompress(compressed, gzSize, uncompressed, size_src * 2);
	printf("uncompressed: %s\n", uncompressed);
	printf("uncompressed size: %d\n", strlen(uncompressed));

	free(compressed);
	free(uncompressed);

	getchar();
	return 0;
}