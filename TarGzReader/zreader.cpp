#include "zreader.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace NewYorkTime;

ZReader::ZReader() :isGood(true), err(""), bufSize(10240), entry(NULL){
	ina = archive_read_new();
	if (ina == NULL){
		err = Error("Couldn't create archive reader.");
		isGood = false;
		cerr << err;
		return;
	}
	if (archive_read_support_filter_all(ina) != ARCHIVE_OK || archive_read_support_format_all(ina) != ARCHIVE_OK){
		err = Error(ina);
		isGood = false;
		cerr << err;
		return;
	}

}
bool ZReader::readFromFile(string fileName){
	if (archive_read_open_filename(ina, fileName.c_str(), bufSize) != ARCHIVE_OK){
		isGood = false;
		err = Error(ina);
	}
	return *this;
}
string ZReader::read(){
	stringstream t;
	char *buf = new char[bufSize];
	if (archive_entry_size(entry) > 0) {
		int len = archive_read_data(ina, buf, bufSize);
		while (len > 0) {
			t.write(buf, len);
			len = archive_read_data(ina, buf, bufSize);
		}
		if (len < 0){
			isGood = false;
			err = Error(ina);
			cerr << err;
		}
	}
	delete[]buf;
	return t.str();
}
bool ZReader::nextEntry(){
	if (archive_read_next_header(ina, &entry) != ARCHIVE_OK){
		isGood = false;
		err = Error(ina);
	}
	return *this;
}
bool ZReader::nextEntry(FileType fileType){
	while (true){
		if (archive_read_next_header(ina, &entry) != ARCHIVE_OK){
			isGood = false;
			err = Error(ina);
			break;
		}
		if (archive_entry_filetype(entry) == fileType)
			break;
	}
	return *this;
}
ZReader::operator bool(){
	if (!isGood)
		cerr << err;
	return isGood;
}