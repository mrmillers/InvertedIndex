#ifndef GZ_READER
#define GZ_READER
#include <vector>
#include <zlib.h>
#include <string>

struct Html{
public:
	Html();
	const char * getHtml() const;
	void setHtml(char *);
	char url[255];
	char ip[20];
	int port,len,n1,n2;
	char status[20];
	~Html();
private:
	char * html;
};

class GzReader{
public:
	GzReader(const char *path,int start = 0);
	bool nextFile();
	Html* nextPage();
	~GzReader();

private:
	char* dir;
	char* file;
	char* inBuffer;
	char* outBuffer;
	FILE* fp;
	int inBufferSize,outBufferSize;
	int fileCnt,pageCnt;
	int err;
	z_stream* zs;
	std::vector<Html *>html;
	long fileSize(FILE *);
	long uncompress(FILE *);
};

#endif 