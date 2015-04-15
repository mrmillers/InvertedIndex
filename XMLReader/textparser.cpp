#include "textparser.h"
#include <cctype>
using namespace NewYorkTime;
using namespace std;
string NewYorkTime::addTagP(const string& s){
	using namespace std;
	//string s = (xml.getRoot().getNodesByTag("head"))[0]->getAllText();
	string t;
	int start = 0;
	while ((start = s.find_first_not_of(" \t\n", start)) != string::npos){
		int end = s.find_first_of(" \t\n", start);
		if (end != string::npos){
			t += s.substr(start, end - start) + " p\n";
		}
		else{
			t += s.substr(start) + " p\n";
			break;
		}
		start = end;
	}
	return t;
}
string NewYorkTime::removePunction(const string& s){
	using namespace std;
	string ret;
	for (int i = 0; i < s.size(); i++){
		if (!ispunct(s[i])){
			ret += s[i];
		}
		else {
			ret += " ";
		}
	}
	return ret;
}
string NewYorkTime::toLower(const string & s){
	string t(s);
	for (int i = 0; i < t.size(); i++)
		t[i] = ::tolower(t[i]);
	return t;
}
