#include "xmlreader.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace NewYorkTime;



int main(){
	ifstream infile("../../TestData/0009873.xml");
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
	cout << "------------------------------------------" << endl;
	const XmlNode * tt = x.getRoot().getNodeByTag("w");
	cout << tt<<endl;


	tt = x.getRoot().getNodeByAttr("name","publication_day_of_week");
	cout << *tt <<endl;
	cout << tt->getAttrValue("content");
	system("pause");
	return 0;
}