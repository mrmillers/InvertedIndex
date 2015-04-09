#include "XMLReader/xmlreader.h"
#include "TarGzReader/files.h"
#include "InvertedIndex/ilist.h"
#include "InvertedIndex/multifile.h"
#include "parser.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;
using namespace NewYorkTime;

static int fileSize = 300 << 20;
string dir = "../../TestData/";

void genHead(){
	MultiFile * list = new MultiFile("../../TestData/list", false);
	list->setFileSize(fileSize);
	list->create();
	delete list;

	InvertedList *il = new InvertedList("../../TestData/list", "../../TestData/tmp");
	il->create();

	int fileCnt = 0, pageCnt = 0;
	fprintf(stderr, "Reading xml files...\n");
	vector<string> xmls = getFiles("*.xml", "../../TestData/xml");
	for (unsigned int i = 0; i < xmls.size(); i++){
		Xml xml(readFile(xmls[i]));
		//cout << i<< xmls[i] << endl;
		string s = (xml.getRoot().getNodesByTag("head"))[0]->getAllText();
		Page p;
		p.content = parse(removePunction(s));
		const XmlNode * node = xml.getRoot().getNodeByTag("title");
		if (node != NULL)
			p.title = node->getAllText();
		else
			p.title = "";
		//cout << "befor feed" << endl;
		il->feed(p);
		//cout << "after feed" << endl;
		fileCnt++;
		if (fileCnt % 1000 == 0)
			printf("%d ", fileCnt);
	}
	il->mergSort(fileSize);
	il->finish();
	delete il;
}

int main(){
	genHead();
	/*string s = "asfi!d0)dfih,,fd df;; \t dfd\ndfih++";
	s = removePunction(s);
	cout << s;
	system("pause");*/
}