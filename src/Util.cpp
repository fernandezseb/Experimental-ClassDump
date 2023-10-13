#include "Util.h"

std::string joinStrings(std::vector<std::string> const& strings, std::string delim)
{
	if (strings.empty()) {
		return std::string();
	}

	std::stringstream ss;

	ss << strings[0];

	if (strings.size() > 1) {
		std::vector<std::string>::const_iterator it = strings.begin() + 1;
		while (it != strings.end()) {
			ss << delim;
			ss << *it;
			it++;
		}
	}

	return ss.str();
}