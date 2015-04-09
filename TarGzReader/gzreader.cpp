#include "gzreader.h"
#include <zlib.h>
#include <string.h>
#include <stdlib.h>

GzReader::GzReader(const char* path,int start){
	dir = (char *)malloc(sizeof(char) * (strlen(path) + 2));
	strcpy(dir,path);
	fileCnt = start;
	inBufferSize = 1024;
	outBufferSize = 1024;
	inBuffer = (char *)malloc(sizeof(char) * inBufferSize);
	outBuffer = (char *)malloc(sizeof(char) * outBufferSize);
	file = (char*)malloc(sizeof(char) * (strlen(dir) + 15));
	zs = new z_stream;
	zs->zalloc = Z_NULL;
	zs->zfree = Z_NULL;

}

GzReader::~GzReader(){
	free(dir);
	free(inBuffer);
	free(outBuffer);
	free(file);
	for (int i=0;i<html.size();i++)
		delete html[i];
	inflateEnd(zs);
	delete zs;
}
bool GzReader::nextFile(){
	long cnt;
	char *s;
	int tmp;
	Html *h;
	pageCnt = 0;

	for (int i=0;i<html.size();i++){
		delete html[i];
	}
	html.clear();

	sprintf(file,"%s/%d_index",dir,fileCnt);
	fp = fopen(file,"rb");
	if (fp==NULL){
		return false;
	}
	cnt = uncompress(fp);
	if (cnt < 0) {
		fprintf(stderr, "%s cannot decode.\n", file);
		fileCnt++;
		return true;
	}
	fclose(fp);
	fp = NULL;
	s = outBuffer;
	

	while (*s){
		h = new Html();
		sscanf(s,"%s%d%d%d%s%d%s",h->url,&(h->n1),&(h->n2),&(h->len),h->ip,&(h->port),h->status);
		//h->setHtml();// = (char *)realloc(h->html,sizeof(char) * (h->len + 10));
		html.push_back(h);
		while (*s && (*s!='\n')) s ++;
		if (*s=='\n') s++;
		//printf("%d %s\n",html.size(),h->url);
	}
	
	sprintf(file,"%s/%d_data",dir,fileCnt);
	fp = fopen(file,"rb");
	if (fp==NULL){
		return false;
	}

	cnt = uncompress(fp);
	if (cnt < 0) {
		fprintf(stderr, "%s cannot decode.\n", file);
		for (int i = 0; i<html.size(); i++)	delete html[i];
		html.clear();
		fileCnt++;
		return true;
	}
	fclose(fp);
	fp = NULL;
	s = outBuffer;
		
	for (int i=0;i<html.size();i++){
		//html[i]->html = (char *)realloc(html[i]->html,sizeof(char) * (html[i]->len + 10));
		html[i]->setHtml(s);
		s += html[i]->len;
	}
	fileCnt++;
	return true;
}


Html* GzReader::nextPage(){
	if (html.size()==pageCnt)
		return NULL;
	return html[pageCnt++];
}


long GzReader::fileSize(FILE * p){
	long lSize;
	fseek (p , 0 , SEEK_END);
	lSize = ftell (p);
	rewind (p);
	return lSize;
}

long GzReader::uncompress(FILE *fp){
	long lSize = fileSize(fp);
	long cnt;
	if (inBufferSize<lSize){
		inBuffer = (char *)realloc(inBuffer,lSize * sizeof(char));
		inBufferSize = lSize;
	}
	zs->avail_in = lSize;
	zs->next_in = (Bytef*)inBuffer;
	zs->next_out = (Bytef*)outBuffer;
	zs->avail_out = outBufferSize;

	fread(inBuffer,1,lSize,fp);

	err = inflateInit2(zs,15 | 32);
	if (err!=Z_OK && zs->msg != NULL){
		printf("%s %d\n",zs->msg,err);
	}
	if (err<0) return -1;
	while (zs->avail_in > 0){
		err = inflate(zs,Z_SYNC_FLUSH);
		if (zs->avail_out == 0){
			outBuffer = (char *)realloc(outBuffer, outBufferSize * 2);
			zs->avail_out = outBufferSize;
			zs->next_out = (Bytef*)(outBuffer + outBufferSize);
			outBufferSize *= 2;
		}
		else if (err < 0)
			return -1;
	}
	cnt = outBufferSize - zs->avail_out;
	outBuffer[cnt] = 0;
	return cnt;
}



Html::~Html(){
	free(html);
}
Html::Html(){
	html = NULL;
}
const char* Html::getHtml()const{
	return html;
}

void Html::setHtml(char *s){
	html = (char*)realloc(html,sizeof(char) * (len + 2));
	strncpy(html,s,len);
	html[len] = 0;
}