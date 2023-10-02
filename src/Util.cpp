#include "Util.h"

std::string joinStrings(std::vector<std::string> const& strings, std::string delim)
{
	if (strings.empty()) {
		return std::string();
	}

	return std::accumulate(strings.begin() + 1, strings.end(), strings[0],
		[&delim](std::string x, std::string y) {
			return x + delim + y;
		}
	);
}