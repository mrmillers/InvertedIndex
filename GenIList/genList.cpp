#include "NewYorkTimeLib/NewYorkTime.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using namespace NewYorkTime;

static int fileSize = 300 << 20;
string dir = "../../TestData/";


inline double bm25(double n, double N, double f, double dlrate = 1, double qf = 1, double r = 0, double R = 0, double k1 = 1.2, double k2 = 100, double b = 0.75){
	double K = k1*((1 - b) + b*dlrate);
	double first = (r + 0.5) / (R - r + 0.5) / (n - r + 0.5) * (N - n - R + r + 0.5);
	double second = (k1 + 1)*f / (K + f);
	double third = (k2 + 1)*qf / (k2 + qf);
	return log10(first) *second * third;
}



void genHead(){
	/*MultiFile * list = new MultiFile("../../TestData/list", false);
	list->setFileSize(fileSize);
	list->create();
	delete list;*/

	InvertedList *il = new InvertedList("../../TestData/list", "../../TestData/tmp");
	il->create();

	int fileCnt = 0, pageCnt = 0;
	fprintf(stderr, "Reading xml files...\n");
	vector<string> xmls = getFiles("*.tgz", "../../TestData");
	for (unsigned int i = 0; i < xmls.size(); i++){
		ZReader z;
		z.readFromFile(xmls[i]);
		while (z.nextEntry(FileType::Regular)){
			Xml xml(z.read());
			string s = (xml.getRoot().getNodesByTag("head"))[0]->getAllText();
			Page p;
			p.content = addTagP(toLower(removePunction(s)));
			const XmlNode * node = xml.getRoot().getNodeByTag("title");
			if (node != NULL)
				p.title = node->getAllText();
			else
				p.title = "";
			il->feed(p);
			fileCnt++;
			if (fileCnt % 1000 == 0)
				printf("%d ", fileCnt);
		}
	}
	il->mergSort(fileSize);
	il->finish();
	delete il;
}


void  query(InvertedList *il, string s){
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
	while (sscanf(c, "%s", ts) != EOF){
		if (v.find(ts) == v.end())
			v[ts] = 1;
		else
			v[ts] ++;
		c += strlen(ts);
		while (isspace(*c)) c++;
	}

	for (map<string, int>::iterator itr = v.begin(); itr != v.end(); itr++) {
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
		for (int i = 1; (i<lp.size()) && ((d = il->nextGEQ(lp[i], did)) == did); i++);
		if (d > did || d == -1) did = d; // not in intersection 
		else
		{
			Score s;
			s.id = did;
			s.score = 0;
			double rate = il->getDocLen(did) / avLen;
			for (int i = 0; i < lp.size(); i++){
				int f = il->getFreq(lp[i]);
				s.score += bm25(n[i], N, f, rate, qf[i]);
				top.push_back(s);
				push_heap(top.begin(), top.end());
				if (top.size()>10) {
					pop_heap(top.begin(), top.end());
					top.pop_back();
				}
			}
			did++;
		}
	}
	for (int i = 0; i < lp.size(); i++) il->closeList(lp[i]);

	std::sort_heap(top.begin(), top.end());
	for (int i = 0; i < top.size() && i<10; i++){
		int id = top[i].id;
		cout << id << "\t" << top[i].score << "\t" << il->getUrl(id) << "\n";
	}
}

void evaluate(){
	InvertedList *il = new InvertedList("../../TestData/list", "../../TestData/tmp");
	il->load();
	cout << endl << "Input your queries, Type QUIT to quit" << endl;
	string line("");
	while (line != "QUIT"){
		getline(cin, line);
		//std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()), end;

		query(il, line);

		//end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		//long long sec = (end - start).count();// -start.count();

		//printf("Time: %lldms\n", sec);
	}
	delete il;
}

int main(){
	//genHead();
	evaluate();
	/*string s = "asfi!d0)dfih,,fd df;; \t dfd\ndfih++";
	s = removePunction(s);
	cout << s;
	system("pause");*/
}