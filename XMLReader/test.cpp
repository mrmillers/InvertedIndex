#include "xmlreader.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace NewYorkTime;



int main(){
	ifstream infile("../../TestData/0001710.xml");
	string s((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
	//cout << s;
	//XmlNode xn();
	//string t = s;
	Xml x(s);
	cout << x;

	cout << endl;
	string w("abd&gt;dfd&lt;&amp;amp;\"\"&quot;&apos;");
	replace(w);
	cout << w << endl;

	cout << x.getRoot().getAllText() << endl;
	cout << "------------------------------------------" << endl;
	vector<const XmlNode*> list(x.getRoot().getNodesByTag("head"));
	for (int i = 0; i < list.size(); i++){
		cout << list[i]->getAllText() << endl;
	}
	system("pause");
	return 0;
}