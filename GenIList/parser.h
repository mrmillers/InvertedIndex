#include "XMLReader/xmlreader.h"
#include <string>
#include <cctype>

namespace NewYorkTime{
	static std::string parse(const std::string& s){
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
	static std::string removePunction(const std::string& s){
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
}