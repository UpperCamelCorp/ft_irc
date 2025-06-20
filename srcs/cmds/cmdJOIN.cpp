#include "../inc/Client.hpp"

void Client::joinCommand(std::string command)
{
	std::istringstream cmdstr(command);

	std::vector<std::string> arglist;
	std::string segment;

	while (std::getline(cmdstr, segment, ' '))
	{
		if (!segment.empty())
			arglist.push_back(segment);
	}
	if (arglist.size() < 3)
}