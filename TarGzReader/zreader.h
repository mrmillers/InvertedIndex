#ifndef GZ_READER
#define GZ_READER

#include "archive.h"
#include "archive_entry.h"
#include <string>



namespace NewYorkTime{
	enum FileType{
		Regular = AE_IFREG,
		Link = AE_IFLNK,
		Directory = AE_IFDIR,
	};
	class ZReader{
	public:
		ZReader();
		bool nextEntry();
		bool nextEntry(FileType fileType);
		void readFromMem();
		bool readFromFile(std::string fileName);
		std::string read();
		~ZReader(){
			//if (entry) archive_entry_free(entry); 
			if (ina) archive_read_free(ina);
		}
		operator bool();
	private:
		class Error{
		public:
			Error(archive* a) :errorNumber(archive_errno(a)){ 
				
				const char * s = archive_error_string(a);
				if (s != NULL)
					errorMessage = s;
			}
			friend std::ostream& operator << (std::ostream& os, const Error&e){
				if (e.errorMessage.length()>0)
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
		//std::stringstream memory;
		int bufSize;
		
	};
}
#endif 