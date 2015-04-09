#include "multifile.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const int MultiFile::endOfFile = -1;

MultiFile::MultiFile(char *path,bool tmp) {
	this->path = (char*)malloc(sizeof(char) * (strlen(path) + 2));
	this->tmpStr = (char *)malloc(sizeof(char) * (strlen(path)+10));
	strcpy(this->path, path);
	fileSize = 300 << 20;

	curPos.fileNum = -1; 
	endPos.fileNum = -1;
	isTmp = tmp;
	fp = NULL;
}

MultiFile::~MultiFile(){
	if (fp!=NULL)
		fclose(fp);
	if (isTmp){
		sprintf(tmpStr,"%s/setting",path);
		remove(tmpStr);
		for (int i=0;i<=endPos.fileNum;i++){
			sprintf(tmpStr,"%s/%d",path,i);
			remove(tmpStr);
		}
	}
	else
		create();
	free(this->path);
	free(tmpStr);
	
}

void MultiFile::create(){
	sprintf(tmpStr,"%s/setting",path);
	FILE* fp = fopen(tmpStr,"wb");
	fprintf(fp,"FileSize:%d\nFileCount:%d\nEndPos:%d\n",fileSize,endPos.fileNum+1,endPos.filePos);
	fclose(fp);

	/*sprintf(tmpStr, "%s/%d", path, 0);
	fp = fopen(tmpStr, "wb");
	fclose(fp);*/
	
}

void MultiFile::load(){
	sprintf(tmpStr,"%s/setting",path);
	FILE* fp = fopen(tmpStr,"rb");
	if (fp==NULL)
		create();
	else{
		fscanf(fp,"FileSize:%d\nFileCount:%d\nEndPos:%d",&fileSize,&endPos.fileNum,&endPos.filePos);
		endPos.fileNum -= 1;
		fclose(fp);
	}
}

bool MultiFile::seek(FilePos pos){
	/*if (pos == curPos)
		return true;*/
	if (pos>=endPos){
		FILE *p;
		for (int i=endPos.fileNum+1;i<=pos.fileNum;i++){
			sprintf(tmpStr,"%s/%d",path,i);
			p = fopen(tmpStr,"wb");
			fclose(p);
		}
		endPos = pos;
	}
	if (curPos.fileNum!=pos.fileNum){
		if (fp!=NULL)
			fclose(fp);
		curPos.fileNum = pos.fileNum;
		curPos.filePos = 0;
		sprintf(tmpStr,"%s/%d",path,curPos.fileNum);
		fp = fopen(tmpStr,"rb+");
	}
	//fseek(fp,pos.filePos,SEEK_SET);
	fseek(fp,pos.filePos - curPos.filePos,SEEK_CUR);
	curPos = pos;
	return true;
}
bool MultiFile::move(long len){
	return seek(getFilePos(curPos,len));
}

void MultiFile::setFileSize(int x){
	this->fileSize = x;
}

void MultiFile::nextFile(){
	if (fp != NULL)
		fclose(fp);
	curPos.fileNum++;
	sprintf(tmpStr, "%s/%d", path, curPos.fileNum);
	fp = fopen(tmpStr, "rb+");
	if (fp == NULL){
		fp = fopen(tmpStr,"wb");
		fclose(fp);
		fp = fopen(tmpStr, "rb+");
	}
	curPos.filePos = 0;
}


long MultiFile::read(void *buf,long len){
	char *buffer = (char *)buf;
	if (fp == NULL){
		FilePos start;
		seek(start);
	}
	if (curPos == endPos){
		return endOfFile;
	}
	if (getFilePos(curPos,len) > endPos){
		len = (endPos.fileNum - curPos.fileNum) * fileSize + (endPos.filePos - curPos.filePos);
	}
	long readed = 0;
	readed = fread(buffer,sizeof(char),len,fp);
	curPos.filePos += readed;
	if (readed<len){
		nextFile();
		readed += read(buffer+readed,len-readed);
	}
	return readed;
}

long MultiFile::write(const void *buf,long len){
	const char *buffer = (const char *)buf;
	if (fp == NULL){
		FilePos start;
		seek(start);
	}
	long writed = 0;
	long writeble = fileSize - curPos.filePos;
	writed = fwrite(buffer,sizeof(char),writeble>len?len:writeble,fp);
	curPos.filePos += writed;
	if (writed < len){
		//move(0);
		nextFile();
		return writed + write(buffer + writed,len-writed);
	}
	if (curPos > endPos)
		endPos = curPos;
	return writed;
}

void MultiFile::flush(){
	if (fp!=NULL){
		fclose(fp);
		sprintf(tmpStr,"%s/%d",path,curPos.fileNum);
		fp = fopen(tmpStr,"rb+");
	}
	create();
}
MultiFile::FilePos MultiFile::getTotalSize(){ return endPos; }

MultiFile::FilePos MultiFile::getFilePos( MultiFile::FilePos start, int len){
	start.filePos += len;
	start.fileNum += start.filePos / fileSize;
	start.filePos %= fileSize;
	return start;
}

MultiFile::FilePos MultiFile::getFilePos(int len){
	FilePos start;
	start.filePos += len;
	start.fileNum += start.filePos / fileSize;
	start.filePos %= fileSize;
	return start;
}

MultiFile::FilePos MultiFile::getCurPos(){
	return curPos;
}


MultiFile::FilePos::FilePos(){ fileNum = 0; filePos = 0; }


bool MultiFile::FilePos::operator == (FilePos &y){ return fileNum == y.fileNum && filePos == y.filePos; };
bool MultiFile::FilePos::operator > (FilePos &y){ return fileNum > y.fileNum || (fileNum == y.fileNum && filePos > y.filePos); };

bool MultiFile::FilePos::operator >= (FilePos &y){ return *this > y || *this == y; };
bool MultiFile::FilePos::operator <= (FilePos &y){ return *this < y || *this == y; };
bool MultiFile::FilePos::operator < (FilePos &y){ return y > *this; };

const char* MultiFile::getDir(){
	return path;
}
