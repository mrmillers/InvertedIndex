#ifndef GZ_READER
#define GZ_READER

#include "archive.h"
#include "archive_entry.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
namespace NewYorkTime{
	enum FileType{
		Regular = AE_IFREG,
		Link = AE_IFLNK,
		Directory = AE_IFDIR,
	};

	class ZReader{
		
	public:

		static ZReader * makeZReader(){
			ZReader * ret = new ZReader();
			ret->ina = archive_read_new();

			if (ret->ina == NULL){
				cerr << Error("Couldn't create archive reader.");
				delete ret;
				return NULL;
			}
			if ( archive_read_support_filter_all(ret->ina) != ARCHIVE_OK || archive_read_support_format_all(ret->ina) != ARCHIVE_OK){
				cerr << Error(ret->ina);
				delete ret;
				return NULL;
			}

			//archive_read_support_filter_gzip(ina);
			//archive_read_support_format_raw(ina);
			//FILE *fp = fopen("../../TestData/0_index.gz","rb");
			//archive_read_set_format(ina,);
			//archive_read_append_filter(ina, ARCHIVE_FORMAT_ZIP);
			//archive_read_append_filter(ina, ARCHIVE_FORMAT_TAR);

			/*if (archive_read_open_filename(ina, "../../TestData/01.tgz", 8192) != ARCHIVE_OK)
				die("Couldn't open input archive");*/

			return ret;
		}
		~ZReader(){ 
			//if (entry) archive_entry_free(entry); 
			if (ina) archive_read_free(ina); }
		operator bool(){
			if (!isGood)
				cerr << err;
			return isGood;
		}


		ZReader() :isGood(true), err("Good."), bufSize(8192),entry(NULL){
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
		bool nextEntry(){
			if (archive_read_next_header(ina, &entry) != ARCHIVE_OK){
				isGood = false;
				err = Error(ina);
			}
			return *this;
		}
		bool nextEntry(FileType fileType){
			
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
		void readFromMem(){}
		bool readFromFile(std::string fileName){
			if (archive_read_open_filename(ina, fileName.c_str(), bufSize) != ARCHIVE_OK){
				isGood = false;
				err = Error(ina);
			}
			return *this;
		}
		std::string read(){
			stringstream t;
			char *buf = new char[bufSize];
			if (archive_entry_size(entry) > 0) {
				int len = archive_read_data(ina, buf, bufSize);
				while (len > 0) {
					t.write(buf,len);
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

	private:
		class Error{
		public:
			Error(archive* a) :errorMessage(archive_error_string(a)),errorNumber(archive_errno(a)){}
			friend std::ostream& operator << (std::ostream& os, const Error&e){
				os << "Error " << e.errorNumber << ": " << e.errorMessage << "\n";
				return os;
			}
			Error(const std::string& em, int en = 0) :errorMessage(em),errorNumber(en){}
		private:
			std::string errorMessage;
			int errorNumber;
		};
		ZReader(const ZReader&);
		archive *ina;
		archive_entry *entry;
		bool isGood;
		Error err;
		std::stringstream memory;
		int bufSize;
		
	};
}
#endif 