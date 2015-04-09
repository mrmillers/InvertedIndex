#include "files.h"
#include "gzreader.h"
using namespace std;

int main(){
	/*vector<string> v = getFiles("*.xml","../../TestData");
	for (int i = 0; i < v.size(); i++){
		cout << v[i] << endl;
	}
	*/
	Html* h;
	GzReader *gz = new GzReader("../../TestData");
	while (gz->nextFile()){
		while (h = gz->nextPage()){
			//pageCnt++;
			//if (pageCnt % 10000 == 0)printf("\n %d ", pageCnt);
			//il->feed(h);
			
			//if (pageCnt == 2) break;
		}
		//printf("%d ", fileCnt++);
	}
	system("pause");
	return 0;
}