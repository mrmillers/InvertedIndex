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

#include <sys/stat.h>
#include "zreader.h"

#include "archive.h"
#include "archive_entry.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace NewYorkTime;
int main(int argc, char **argv){
	{
		ZReader z;
	}
	ZReader z;
	z.readFromFile("../../TestData/01.tgz");
	string t;
	for (int i = 0; i < 10;i++){
		z.nextEntry(FileType::Regular);
		t = z.read();
		cout << t << endl;
	}
	return 0;
}
