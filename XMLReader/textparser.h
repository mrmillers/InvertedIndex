#include <string>
#include <cctype>

namespace NewYorkTime{
	std::string parse(const std::string& s);
	std::string removePunction(const std::string& s);
	std::string toLower(const std::string& s);
}