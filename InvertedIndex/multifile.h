#ifndef MULTI_FILE
#define MULTI_FILE
#include <stdio.h>

class MultiFile{
public:
	struct FilePos{
		FilePos();
		int fileNum, filePos;
		void set(int num, int pos){ fileNum = num > 0 ? num : 0; filePos = pos > 0 ? pos : 0; };

		bool operator == (FilePos &y);// { return fileNum == y.fileNum && filePos == y.filePos; };
		bool operator > (FilePos &y);// { return fileNum > y.fileNum || (fileNum == y.fileNum && filePos > y.filePos); };

		bool operator >= (FilePos &y);// { return *this > y || *this == y; };
		bool operator <= (FilePos &y);// { return *this < y || *this == y; };
		bool operator < (FilePos &y);// { return y > *this; };
	};
	MultiFile(char *path,bool tmp = false);
	~MultiFile();
	void load();
	void setFileSize(int size);
	void create();
	long read(void *buffer,long len);
	long write(const void *buffer,long len);
	bool seek(FilePos pos);
	bool move(long len);
	const char *getDir();
	static const int endOfFile;
	FilePos getTotalSize();
	void flush();
	FilePos getFilePos(FilePos start, int len);
	FilePos getFilePos(int len);
	FilePos getCurPos();
private:
	FILE *fp;
	char *path, *tmpStr;
	int fileSize;
	FilePos endPos,curPos;
	bool isTmp;
	void nextFile();
};


#endif