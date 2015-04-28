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


void evaluate(){
	InvertedList *il = new InvertedList("../../TestData/list");
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
	genHead();
	evaluate();
	/*string s = "asfi!d0)dfih,,fd df;; \t dfd\ndfih++";
	s = removePunction(s);
	cout << s;
	system("pause");*/
}