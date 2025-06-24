#include "Irc.hpp"

std::vector<std::string>	split_cmd(std::string str, char sep)
{
	std::istringstream cmdstr(str);
	std::vector<std::string> arglist;
	std::string segment;
	while (std::getline(cmdstr, segment, sep))
	{
		while (!segment.empty() && (segment[segment.size() - 1] == '\n'
				|| segment[segment.size() - 1] == '\r'))
			segment.resize(segment.size() - 1);
		if (!segment.empty())
			arglist.push_back(segment);
	}
	return (arglist);
}
