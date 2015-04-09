#ifndef MY_FILES
#define MY_FILES
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
namespace NewYorkTime{
	static std::vector<std::string> getFiles(std::string names, std::string dir = "."){
		std::string cmd = "cd " + dir + "\ndir /b /s " + names + " > tmp";
		std::ofstream outf("script.bat");
		outf << cmd;
		outf.close();
		system("script.bat");
		std::ifstream inf(dir + "/tmp");
		std::vector<std::string> v;
		std::string c;
		while (std::getline(inf, c)){
			v.push_back(c);
		}

		return v;
	}

	static std::string readFile(std::string fileName){
		std::ifstream infile(fileName);
		return std::string((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
		
	}
}

#endif // MY_FILES