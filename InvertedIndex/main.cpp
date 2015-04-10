#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <crtdbg.h>
#include <cstdio>
#include <vector>
#include <queue>
#include <string>
#include "gzreader.h"
#include "compresser.h"
#include "multifile.h"
#include "ilist.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
static int fileSize = 300<<20;
static bool isDebug = false;
static InvertedList *il = NULL;//new InvertedList("../data/list", "../data/tmp", isDebug);

/*
void gen(){
	MultiFile * list = new MultiFile("../data/list",false);
	list->setFileSize(fileSize);
	list->create();
	delete list;

	InvertedList *il = new InvertedList("../data/list","../data/tmp",isDebug);
	il->create();
	GzReader *gz = new GzReader("../data/pages");
	
	Html* h;
	int fileCnt = 0,pageCnt = 0;
	
	fprintf(stderr,"Reading gz files...\n");
	while (gz->nextFile()){
		while (h = gz->nextPage()){
			pageCnt++;
			if (pageCnt % 10000 == 0)printf("\n %d ",pageCnt);
			il->feed(h);

			//if (pageCnt == 2) break;
		}
		printf("%d ",fileCnt++);
	}
	//il->bucketSort();
	il->mergSort(fileSize);
	il->finish();
	delete gz;
	delete il;
}

inline double bm25(double n, double N, double f, double dlrate = 1, double qf = 1, double r = 0, double R = 0, double k1 = 1.2, double k2 = 100, double b = 0.75){
	double K = k1*((1 - b) + b*dlrate);
	double first = (r + 0.5) / (R - r + 0.5) / (n - r + 0.5) * (N - n - R + r + 0.5);
	double second = (k1 + 1)*f / (K + f);
	double third = (k2 + 1)*qf / (k2 + qf);
	return log10(first) *second * third;
}
*/
/*
void query(string s){
	struct Score{
		int id;
		double score;
		bool operator <(const Score& x)const { return this->score > x.score; }
	};
	if (s.length() == 0)
		return;

	cout << "DocID\tScore\tUrl\n";
	map<string, int> v;
	vector<int> qf;// [100] = { 0 };
	vector<int> lp;// [100] = { 0 };
	int n[100] = { 0 };
	
	//vector<string> v;
	const char *c = s.c_str();
	char ts[256];
	vector<Score> top;
	while (sscanf(c,"%s",ts)!=EOF){
		if (v.find(ts) == v.end())
			v[ts] = 1;
		else
			v[ts] ++;
		c += strlen(ts);
		while (isspace(*c)) c++;
	}

	for (map<string,int>::iterator itr = v.begin(); itr != v.end(); itr++) { 
		lp.push_back(il->openList(itr->first.c_str()));
		qf.push_back(itr->second);
	}
	for (int i = 0; i < lp.size(); i++) n[i] = il->getDocCnt(lp[i]);

	int did = 0;
	int total = n[0];
	int N = il->getTotalDoc();
	double avLen = static_cast<double>(il->getTotalLen()) / N;
	while ((did = il->nextGEQ(lp[0], did)) != -1){
		//total--;
		//if (total % 100 == 0) cout << total <<" "<< did << endl;
		int d = 0;
		for (int i = 1; (i<lp.size()) && (( d = il->nextGEQ(lp[i], did)) == did); i++);
		if (d > did || d == -1) did = d; // not in intersection 
		else
		{
			Score s;
			s.id = did;
			s.score = 0;
			double rate = il->getDocLen(did) / avLen;
			for (int i = 0; i < lp.size(); i++){
				int f = il->getFreq(lp[i]);
				s.score += bm25(n[i], N, f,rate,qf[i]);
				top.push_back(s);
				push_heap(top.begin(),top.end());
				if (top.size()>10) {
					pop_heap(top.begin(), top.end());
					top.pop_back();
				}
			} 
			did++; 
		}
	}
	for (int i = 0; i < lp.size(); i++) il->closeList(lp[i]);

	std::sort_heap(top.begin(),top.end());
	for (int i = 0; i < top.size() && i<10; i++){
		int id = top[i].id;
		cout << id << "\t" << top[i].score << "\t" << il->getUrl(id) << "\n";
	}
}
void test(){
	il = new InvertedList("../data/list", "../data/tmp", isDebug);
	il->load();
	for (int i = 0; i < 5; i++){
		char s[1024];
		scanf("%s", s);

		long long docT = 0, wordT = 0;
		std::chrono::milliseconds start = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()), end;

		int id = il->openList(s), docID = 0;
		while ((docID = il->nextGEQ(id, docID)) != -1){
			int f = il->getFreq(id);
			//printf("\n%d %d\n",docID,f);
			docT += 1;
			wordT += f;
			docID++;
		}


		end = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch());
		long long sec = (end - start).count();// -start.count();


		printf("\n%lld %lld %lld\n", wordT, docT, sec);
		il->closeList(id);
	}

	delete il;
}
int main (int argc, char * argv[])
{
	//gen(); exit(0);
	
	//test(); exit(0);

	il = new InvertedList("../data/list", "../data/tmp", isDebug);
	il->load();
	cout << endl << "Input your queries, Type QUIT to quit" << endl;
	string line("");
	while (line!="QUIT"){
		getline(cin, line);
		std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::high_resolution_clock::now().time_since_epoch()), end;

		query(line);

		end = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch());
		long long sec = (end - start).count();// -start.count();
		
		printf("Time: %lldms\n",sec);
	}
	delete il;
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}
*/
int main(){

}