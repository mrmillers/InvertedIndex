#ifndef INVERTED_LIST
#define INVERTED_LIST

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "multifile.h"
#include "compresser.h"
namespace NewYorkTime{
	struct Page{
		std::string title;
		std::string content;
	};

	class InvertedList{
	public:
		InvertedList(char * listDir, char *tmpDir = NULL, bool isDebug = false);
		~InvertedList();

		//for generate 
		void create();
		void feed(const Page&);
		void mergSort(int maxLen = 100 << 20);
		void finish();

		//for query
		void load();
		int openList(const char * word);
		void closeList(int id);
		int nextGEQ(int id, int docID);
		int getFreq(int id)const;

		int getDocCnt(int id)const;
		int getTotalDoc()const{ return idUrl.size(); }

		int getDocLen(int id)const{ return idLen[id]; }
		int getTotalLen()const { return totalDocLen; }
		char * getUrl(int id)const{ return idUrl[id]; }


	private:
		struct ListItem{
			int wordId;
			char *word;
			std::vector<int> tag;
			std::vector<int> pos;
		};

		/*struct cmp_str{
			bool operator() (char *a, char *b){
				return strcmp(a, b) < 0;
			}
			

		};*/
		struct WordInfo{
			WordInfo();
			//~WordInfo();
			long blockSize;
			MultiFile::FilePos start;
			int docCnt;
			int wordCnt;
		};

		struct DocInfo{
			int docID, wordCnt;
			int *tag;
			int *pos;
			DocInfo* next;
		};
		struct ListHead{
			DocInfo* first, *cur;
		};


		bool isDebug, isHtml;
		int urlCnt, blockCnt, totalDocLen;
		char * path, *tmpStr;

		Compresser *com;
		MultiFile* tmpFile, *listFile;
		FILE *iUrl, *iLex, *fDebug;

		std::map<std::string, int> wordsID;
		std::map<int, ListHead> idDocList;
		std::vector<char *>idWords;
		std::vector<WordInfo> wordsList;
		std::vector<char *> idUrl;
		std::vector<int>idLen;


		int getTag(const char *);
		char *copyStr(const char *);
	};

}

#endif