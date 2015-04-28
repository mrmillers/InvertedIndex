#include "NewYorkTimeLib/NewYorkTime.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using namespace NewYorkTime;

static int fileSize = 300 << 20;
string dir = "../../Data/";

void genHead(){

	InvertedList *il = new InvertedList("../../listHead", "../../TestData/tmp");
	il->create();

	int pageCnt = 0;
	fprintf(stderr, "Reading xml files...\n");
	vector<string> tgz = getFiles("*.tgz", dir.c_str());
	for (unsigned int i = 0; i < tgz.size(); i++){
		ZReader z;
		z.readFromFile(tgz[i]);
		while (z.nextEntry(FileType::Regular)){
			Xml xml(z.read());
			string s = xml.getRoot().getNodeByTag("head")->getAllText();
			Page p;
			p.content = addTagP(toLower(removePunction(s)));
			const XmlNode * node = xml.getRoot().getNodeByTag("title");
			if (node != NULL)
				p.title = node->getAllText();
			else
				p.title = "";
			il->feed(p);
			pageCnt++;
			if (pageCnt % 1000 == 0)
				printf("%d ", pageCnt);
		}
		printf("%d\n",tgz.size()-i);
	}
	il->mergSort(fileSize);
	il->finish();
	delete il;
}
void genBody(){
	InvertedList *il = new InvertedList("../../listBody", "../../TestData/tmp");
	il->create();

	int pageCnt = 0;
	fprintf(stderr, "Reading xml files...\n");
	vector<string> tgz = getFiles("*.tgz", dir.c_str());
	for (unsigned int i = 0; i < tgz.size(); i++){
		ZReader z;
		z.readFromFile(tgz[i]);
		while (z.nextEntry(FileType::Regular)){
			Xml xml(z.read());
			string s = xml.getRoot().getNodeByTag("body")->getAllText();
			Page p;
			p.content = addTagP(toLower(removePunction(s)));
			const XmlNode * node = xml.getRoot().getNodeByTag("title");
			if (node != NULL)
				p.title = node->getAllText();
			else
				p.title = "";
			il->feed(p);
			pageCnt++;
			if (pageCnt % 1000 == 0)
				printf("%d ", pageCnt);
		}
		printf("%d\n", tgz.size() - i);
	}
	il->mergSort(fileSize);
	il->finish();
	delete il;
}

void genSQL(){

	//SQL sql_n("tcp://127.0.0.1:3306", "root", "", "nyt");
	//SQL sql_a("tcp://127.0.0.1:3306", "root", "", "nyt");
	//SQL sql_l("tcp://127.0.0.1:3306", "root", "", "nyt");
	//SQL sql_t("tcp://127.0.0.1:3306", "root", "", "nyt");


}
void evaluate(){
	InvertedList *il = new InvertedList("../../listHead");
	il->load();
	cout << endl << "Input your queries, Type QUIT to quit" << endl;
	string line("");
	while (line != "QUIT"){
		getline(cin, line);
		//std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()), end;
		cout << "starting query " << line << endl;
		cout << il->query(20, line);
		//end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		//long long sec = (end - start).count();// -start.count();

		//printf("Time: %lldms\n", sec);
	}
	delete il;
}

int main(){
	//genHead();
	//genBody();
	evaluate();
	
}