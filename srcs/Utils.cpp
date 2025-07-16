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

std::string trim_cmd(const std::string& input)
{
    size_t end = input.size();
    while (end > 0 && (input[end - 1] == '\n' || input[end - 1] == '\r'))
	{
        --end;
    }
    return input.substr(0, end);
}

bool	valid_channel_name(const std::string& str){
	if (str.empty() || str[0] != '#' || str.length() > 50)
		return false;
	if (str.find(':') != std::string::npos)
		return false;
	return true;
}