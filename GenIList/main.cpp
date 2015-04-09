#include "XMLReader/xmlreader.h"
#include "TarGzReader/files.h"
#include "InvertedIndex/ilist.h"
#include "InvertedIndex/multifile.h"
#include <string>
#include <fstream>
#include <vector>


using namespace std;
using namespace NewYorkTime;

static int fileSize = 300 << 20;
static bool isDebug = false;
string dir = "../../TestData/";

int main(){
	MultiFile * list = new MultiFile("../../TestData/list", false);
	list->setFileSize(fileSize);
	list->create();
	delete list;

	InvertedList *il = new InvertedList("../../TestData/list", "../../TestData/tmp", isDebug);
	il->create();
	
	int fileCnt = 0, pageCnt = 0;
	fprintf(stderr, "Reading xml files...\n");
	vector<string> xmls = getFiles("*.xml", "../../TestData");
	for (unsigned int i = 0; i < xmls.size();i++){
		Xml xml(readFile(xmls[i]));
		string s = xml.getRoot().getAllText();
		//il->feed();
		printf("%d ", fileCnt++);
	}
	il->mergSort(fileSize);
	il->finish();
	delete il;
}