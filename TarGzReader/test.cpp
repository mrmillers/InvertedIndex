/*
* This file is in the public domain.
*
* Feel free to use it as you wish.
*/

/*
* This example program reads an archive from stdin (which can be in
* any format recognized by libarchive) and writes certain entries to
* an uncompressed ustar archive on stdout.  This is a template for
* many kinds of archive manipulation: converting formats, resetting
* ownership, inserting entries, removing entries, etc.
*
* To compile:
* gcc -Wall -o tarfilter tarfilter.c -larchive -lz -lbz2
*/
#include "zreader.h"
#include <iostream>

using namespace std;
using namespace NewYorkTime;
int main(int argc, char **argv){
	{
		ZReader z;
		//archive_errno(NULL);
	}
	ZReader z;
	z.readFromFile("../../TestData/01.tgz");
	string t;
	int n = 0;
	while (z.nextEntry(FileType::Regular)){
		
		t = z.read();
		++n;

		//cout << t << endl;
	}
	return 0;
}
