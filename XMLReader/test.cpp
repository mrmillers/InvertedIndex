#include "xmlreader.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace NewYorkTime;



int main(){
	ifstream infile("../../TestData/0001503.xml");
	string s((istreambuf_iterator<char>(infile)),istreambuf_iterator<char>());
	//cout << s;
	//XmlNode xn();
	//string t = s;
	Xml x(s);
	cout << x;
	system("pause");
	return 0;
}