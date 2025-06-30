#include "Irc.hpp"
#include "Client.hpp"

void Client::joinCommand(std::string command)
{
	std::istringstream cmdstr(command);
	std::vector<std::string> arglist;
	std::string segment;
	while (std::getline(cmdstr, segment, ' '))
	{
		while (!segment.empty() && (segment[segment.size() - 1] == '\n'
				|| segment[segment.size() - 1] == '\r'))
			segment.resize(segment.size() - 1);
		if (!segment.empty())
		{
			arglist.push_back(segment);
		}
	}
}
