#include "files.h"
using namespace std;

int main(){
	vector<string> v = getFiles("*.xml","../../TestData");
	for (int i = 0; i < v.size(); i++){
		cout << v[i] << endl;
	}
	//system("dir");
	system("pause");
	return 0;
}