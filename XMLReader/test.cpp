#include "xmlreader.h"
#include <iostream>
using namespace std;
using namespace NewYorkTime;
int main(){
	XmlNode xn("<a abc=\"f\"><b></b></a>");
	cout << xn;
	system("pause");
	return 0;
}