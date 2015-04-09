#include <string.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <ctype.h>
#include "ilist.h"
#include "parser.h"



InvertedList::InvertedList(char *listDir,char *tmpDir,bool isDebug){
	com = new Simple9();
	this->isDebug = isDebug;
	//idWords = NULL;
	fDebug = NULL;
	urlCnt = 0;
	blockCnt = 0;
	
	path = (char*)malloc(sizeof(char) * (strlen(listDir)+2));
	strcpy(path,listDir);
	tmpStr = (char*)malloc(sizeof(char)*(strlen(path) + 20));
	
	
	tmpFile = new MultiFile(tmpDir,!isDebug);
	tmpFile->create();
	listFile = new MultiFile(path);
	listFile->load();
	
	
}
InvertedList::~InvertedList(){
	if (iUrl!=NULL) fclose(iUrl);
	if (iLex!=NULL) fclose(iLex);
	if (fDebug!=NULL) fclose(fDebug);
	free(path);
	free(tmpStr);	
	for (int i = 0; i < idWords.size(); i++)
		free(idWords[i]);
	for (int i = 0; i < idUrl.size(); i++)
		free(idUrl[i]);
	for (std::map<int,ListHead>::iterator it = idDocList.begin(); it != idDocList.end(); it++){
		closeList(it->first);
	}

	delete tmpFile;
	delete listFile;
	delete com;
	
}

void InvertedList::feed(const Html* html){
	//fprintf(iUrl, "%d %s\n", urlCnt++, html->url);
	urlCnt++;
	char *back  = (char *)malloc(sizeof(char)*html->len * 2);
	char *lexs = back;
	char* url = copyStr(html->url);
	char* page = copyStr(html->getHtml());
	
	*lexs = 0;
	parser(url,page,lexs,html->len*2);
	free(url);
	free(page);

	std::map<int,std::vector<int>>docWord;
	int id,pos = 0;
	char* tag,*lex;
	while (*lexs){
		lex = lexs;
		while (*lexs && (*lexs != '\n')){
			if (isspace(*lexs)) *lexs = 0;
			lexs++;
		}
		for (tag = lexs - 1; *(tag-1); tag--);
		if (*lexs) *(lexs++) = 0;
		
		
		/*lex = lexs;
		while (*lexs && (*lexs != ' ')) lexs++;
		if (*lexs) *(lex++) = 0;
		tag = lexs;
		while (*lexs && (*lexs != '\n')) lexs++;
		if (*lexs) *(lex++) = 0;
		*/
		if (tag - lex > 255) lex[255] = 0;
		if (wordsID.find(lex)!=wordsID.end()){
			id = wordsID.find(lex)->second;
		}
		else{
			id = wordsList.size();
			WordInfo wi;
			wordsList.push_back(wi);
			idWords.push_back(copyStr(lex));
			wordsID[idWords[id]] = id;
			//fprintf(iLex, "%d %s %s\n", id, lex, tag);
		}
		
		docWord[id].push_back(getTag(tag));
		docWord[id].push_back(pos);
		pos++;
	}

	fprintf(iUrl, "%d %d %s\n", urlCnt,pos, html->url);

	unsigned int *buffer = NULL;
	unsigned int *outBuffer = NULL;
	for (std::map<int,std::vector<int>>::iterator it=docWord.begin(); it!=docWord.end(); ++it){
		int iCnt = it->second.size() / 2;  //# of intergers
		
		buffer = (unsigned int *)realloc(buffer,sizeof(unsigned int)*(iCnt * 2 + 10));
		outBuffer = (unsigned int *)realloc(outBuffer,sizeof(unsigned int)*iCnt*4);
		buffer[0] = urlCnt - 1;
		buffer[1] = iCnt;
		for (int i=0;i<iCnt;i++){
			buffer[i+2] = it->second[i*2];
			buffer[iCnt+i+2] = it->second[i*2+1];
		}
		int size = com->compress(buffer,outBuffer,iCnt * 2 + 2,iCnt*4) * sizeof(unsigned int); //# of char store in a int

		wordsList[it->first].docCnt ++;
		wordsList[it->first].wordCnt += iCnt;
		wordsList[it->first].blockSize += size;
		
		//file format : wordID, blockLen, Block( docId,cnt,tag0,pos0,ta1,pos1... )
		tmpFile->write(&(it->first),sizeof(unsigned int));//id
		tmpFile->write(&size,sizeof(unsigned int));//blockLen
		tmpFile->write(outBuffer,size);//block
		
		if (isDebug){
			fprintf(fDebug,"%d %s %d:",it->first,idWords[it->first],urlCnt-1);
			for (int k=0;k<iCnt;k++){
				fprintf(fDebug,"%d ",it->second[k*2+1]);
			}
			fprintf(fDebug,"\n");
		}
		blockCnt++;
	}
	free(buffer);
	free(outBuffer);
	free(back);//backup pointer for lexs

}



int InvertedList::getTag(const char *tag){ //need update
	switch (*tag)
	{
	case 'B':
	case 'b': 
		return 1;
		break;
	case 'H':
	case 'h':
		return 2;
		break;
	case 'I':
	case 'i':
		return 3;
		break;
	case 'T':
	case 't':
		return 4;
		break;
	case 'U':
	case 'u':
		return 5;
		break;
	default:
		break;
	}
	return 0;
}

InvertedList::WordInfo::WordInfo(){
	blockSize = 0;
	docCnt = 0;
	wordCnt = 0;
}

char *InvertedList::copyStr(const char * s){
	char *ret = (char*)malloc(sizeof(char)*(strlen(s)+2));
	strcpy(ret,s);
	return ret;
}

void InvertedList::create(){
	sprintf(tmpStr, "%s/iUrl", path);
	iUrl = fopen(tmpStr, "wb");
	sprintf(tmpStr, "%s/iLex", path);
	iLex = fopen(tmpStr, "wb");

	if (isDebug){
		sprintf(tmpStr, "%s/iDebug", path);
		fDebug = fopen(tmpStr, "wb");
	}
}

void InvertedList::load(){
	//sprintf(tmpStr,"%s/iLex",path);
	//FILE* fp = fopen(tmpStr,"rb");
	sprintf(tmpStr, "%s/iUrl", path);
	iUrl = fopen(tmpStr, "rb+");
	sprintf(tmpStr, "%s/iLex", path);
	iLex = fopen(tmpStr, "rb+");

	if (isDebug){
		sprintf(tmpStr, "%s/lDebug", path);
		fDebug = fopen(tmpStr, "wb");
	}


	int id, fileN, fileP,pos;
	WordInfo w;
	char s[102400];
	fseek(iLex, 0, SEEK_SET);
	fprintf(stderr,"Loading Vocabulary\n");
	while (fscanf(iLex, "%d%d%d%d%d%d%s", &id, &fileN, &fileP, &w.blockSize,&w.wordCnt,&w.docCnt, s) != EOF){
		idWords.push_back(copyStr(s));
		wordsID[idWords[id]] = id;
		w.start.set(fileN,fileP);
		wordsList.push_back(w);
		//fscanf(iLex,"\n");
		//fgets(s,1024,iLex);
		//printf("%d ",id);
		if (id % 10000 == 0) fprintf(stderr,"%d ", id);
		//if (id > 2060000) fprintf(stderr,"\n %d",id);
	}
	fseek(iLex, 0, SEEK_SET);

	fseek(iUrl, 0, SEEK_SET);
	fprintf(stderr, "Loading Url\n");
	totalDocLen = 0;
	while (fscanf(iUrl, "%d%d%s", &id,&pos, s) != EOF){
		idUrl.push_back(copyStr(s));
		idLen.push_back(pos);
		totalDocLen += pos;
		if (id % 10000 == 0) fprintf(stderr, "%d ", id);
	}
	fseek(iUrl, 0, SEEK_SET);
	//fclose(fp);
}


int InvertedList::openList(const char *wordc){
	char * word = const_cast<char *>(wordc);
	if (wordsID.find(word) == wordsID.end())
		return -1;
	int id = wordsID[word];
	if (idDocList.find(id) != idDocList.end()){
		//fprintf(stderr,"%d found.\n",id);
		idDocList[id].cur = idDocList[id].first;
		return id;
	}
	//if (){}
	
	
	listFile->seek(wordsList[id].start);
	listFile->read(&(wordsList[id].wordCnt),sizeof(int));
	listFile->read(&(wordsList[id].docCnt),sizeof(int));

	//fprintf(stderr,"%d %d %d\n",wordsList[id].wordCnt,wordsList[id].docCnt,wordsList[id].blockSize);

	unsigned int * block = (unsigned int *)malloc(sizeof(char) * wordsList[id].blockSize);
	listFile->read(block,wordsList[id].blockSize);
	
	unsigned int * out = (unsigned int *)malloc(sizeof(int)*1024);

	int len = com->uncompress(block,out,wordsList[id].blockSize / sizeof(int),1024);
	//free(block);

	DocInfo* head = NULL,*p = NULL,*end = NULL;
	for (int i = 0; i < len;){
		//if (i % 10000 == 0) fprintf(stderr, "%d ", len - i);	
		p = new DocInfo();
		p->next = NULL;
		p->docID = out[i++];
		p->wordCnt = out[i++];
		p->pos = NULL;// new int[p->wordCnt];
		p->tag = NULL;// new int[p->wordCnt];
		/*for (int j = 0; j < p->wordCnt; j++){
			p->tag[j] = out[i + j];
			p->pos[j] = out[i + j + p->wordCnt];
		}*/
		i += p->wordCnt * 2;
		if (head == NULL)
			head = p;
		else
			end->next = p;
		end = p;
	}

	idDocList[id].first = head;
	idDocList[id].cur = head;
	free(out);
	free(block);
	return id;
}

void InvertedList::closeList(int id){
	if (idDocList.find(id) == idDocList.end())
		return;
	DocInfo *p = idDocList[id].first,*q;
	while (p != NULL){
		delete [] p->pos;
		delete [] p->tag;
		q = p->next;
		delete p;
		p = q;
	}
	idDocList.erase(id);
}

int InvertedList::nextGEQ(int id, int docID){
	if (docID == -1 || idDocList.find(id)==idDocList.end())
		return -1;
	while (idDocList[id].cur && idDocList[id].cur->docID<docID){
		idDocList[id].cur = idDocList[id].cur->next;
	}
	if (idDocList[id].cur)
		return idDocList[id].cur->docID;
	return -1;
}

int InvertedList::getFreq(int id)const{
	if (idDocList.find(id) == idDocList.end())
		return -1;
	if ((idDocList.find(id))->second.cur == NULL)
		return -1;
	return (idDocList.find(id))->second.cur->wordCnt;
}

int InvertedList::getDocCnt(int id)const{
	return wordsList[id].docCnt;
}


void InvertedList::mergSort(int maxLen){
	struct TmpData{
		TmpData(){ data = NULL; }
		~TmpData(){ free(data); }
		int id,index,len;
		char *data;
		bool operator  () (const TmpData* x, const TmpData * y){
			return (x->id > y->id) || (x->id == y->id && x->index > y->index);
		}
		
	};

	int curLen = 0, index = 0;
	std::vector<FILE*> files;
	std::priority_queue <TmpData*,std::vector<TmpData*>,TmpData> data;
	TmpData *t = NULL;
	tmpFile->seek(tmpFile->getFilePos(0));

	fprintf(stderr,"Start Sort\n");
	while (true){
		//fprintf(stderr,"%d ",blockCnt - data.size());
		t = new TmpData();
		int rFlag = tmpFile->read(&t->id, sizeof(int));;
		tmpFile->read(&t->len, sizeof(int));
		if ((rFlag == MultiFile::endOfFile) || (data.size() > 0 && curLen + t->len + 2*sizeof(int) > maxLen)){
			//start sort current file
			fprintf(stderr,"%d ",files.size());
			sprintf(tmpStr, "%s/s%d", tmpFile->getDir(), files.size());
			FILE*p = fopen(tmpStr, "wb");
			while (!data.empty()){
				TmpData* td = data.top();
				fwrite(&td->id, sizeof(int), 1, p);
				fwrite(&td->len, sizeof(int), 1, p);
				fwrite(td->data, sizeof(char), sizeof(char)*td->len, p);
				data.pop();
				delete td;
			}
			fclose(p);
			p = fopen(tmpStr, "rb");
			files.push_back(p);
			//data.clear();
			curLen = 0;

		}
		if (rFlag == MultiFile::endOfFile){
			delete t;
			//t = NULL;
			break;
		}

		t->index = index++;
		t->data = (char*)malloc(sizeof(char) * t->len);
		tmpFile->read(t->data,t->len * sizeof(char));
		curLen += t->len + 2*sizeof(int);
		data.push(t);
		//t = NULL;
		//std::push_heap(data.begin(),data.end());
	}
	//mergeSort
	fprintf(stderr,"Start Merge\n");
	for (int i = 0; i < files.size(); i++){
		t = new TmpData();
		fread(&t->id, sizeof(int), 1, files[i]);
		fread(&t->len, sizeof(int), 1, files[i]);
		t->index = i;
		t->data = (char*)malloc(sizeof(char) * t->len);
		fread(t->data,sizeof(char),t->len,files[i]);
		data.push(t);
		//t = NULL;
		//std::push_heap(data.begin(),data.end());
	}
	
	listFile->seek(listFile->getFilePos(0));
	int lastID = -1;
	int tot = 0;
	while (!data.empty()){
		TmpData* td = data.top();
		if (lastID != td->id){ //new wordID
			wordsList[td->id].start = listFile->getCurPos();
			listFile->write(&wordsList[td->id].wordCnt,sizeof(int));
			listFile->write(&wordsList[td->id].docCnt, sizeof(int));
			
			if (tot % 10000 == 0) fprintf(stderr,"%d words left\n",idWords.size()-++tot);
		}
		lastID = td->id;
		listFile->write(td->data,td->len);
		int fN = td->index;
		FILE* f = files[td->index];
		data.pop();
		delete td;
		//td = NULL;
		t = new TmpData();
		if (fread(&t->id, sizeof(int), 1, f) != 0){
			fread(&t->len, sizeof(int), 1, f);
			t->index = fN;
			t->data = (char*)malloc(sizeof(char) * t->len);
			fread(t->data, sizeof(char), t->len, f);
			data.push(t);
		}
		else
			delete t;
		//t = NULL;
	}

	for (int i = 0; i < files.size(); i++) { 
		fclose(files[i]); 
		sprintf(tmpStr, "%s/s%d", tmpFile->getDir(), i);
		remove(tmpStr);
	}
}


void InvertedList::finish(){
	fclose(iUrl);
	iUrl = NULL;

	fprintf(stderr, "Saving Vocabulary...\n");

	fseek(iLex, 0, SEEK_SET);
	for (int i = 0; i<wordsList.size(); i++){
		fprintf(iLex, "%d %d %ld %ld %ld %ld %s\n", i, wordsList[i].start.fileNum, wordsList[i].start.filePos, wordsList[i].blockSize,wordsList[i].wordCnt,wordsList[i].docCnt, idWords[i]);
		if (i % 10000 == 0)
			fprintf(stderr, "%d words left\n", wordsList.size() - i);
	}
	fclose(iLex);
	iLex = NULL;
}