#include "../inc/Client.hpp"
#include "Server.hpp"

static void	ErrInvalid(int error_n, std::string err_arg, int socket_fd)
{
	std::string response;
	if (error_n == 461)
		response = ":localhost 461 " + err_arg + " :No enough parameters\n";
	else if (error_n == 403)
		response = ":localhost " + err_arg + " :No such channel\n"; // channel
	else if (error_n == 405)
		response = ":localhost " + err_arg + " :You have joined too many channels\n"; // channel
	else if (error_n == 475)
		response = ":localhost " + err_arg + " :Cannot join channel (+k)\n";
	else if (error_n == 474)
		response = ":localhost " + err_arg + " :Cannot join channel (+b)\n";
	else if (error_n == 471)
		response = ":localhost " + err_arg + " :Cannot join channel (+l)\n";
	else if (error_n == 473)
		response = ":localhost " + err_arg + " :Cannot join channel (+i)\n";
	else if (error_n == 476)
		response = ":localhost " + err_arg + " :Cannot join channel (+b)\n";
	else
		return ;
	send(socket_fd, response.c_str(), response.length(), 0);
}

static std::vector<std::string>	split_cmd(std::string str, char sep)
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

void Client::joinCommand(std::string command)
{
	std::vector<std::string> arglist;
	arglist = split_cmd(command, ' ');
	if (arglist.size() < 2)
		return (ErrInvalid(461, this->getNick(), this->_socket_fd));

	std::vector<std::string> channels;
	channels = split_cmd(arglist[1], ',');

	if (arglist.size() == 3)
	{
		std::vector<std::string> passwords;
		passwords = split_cmd(arglist[2], ',');
	}
	std::map<std::string, Channel> serverChannels;
	serverChannels = this->_server->getChannels();
	std::map<std::string, Channel>::iterator it;

	int i = 0;
	int pass_i = 0;
	while (i < channels.size())
	{
		if (serverChannels.find(channels[0]) != serverChannels.end())
		{
			it = serverChannels.find(channels[0]); 										// exist
			it->second.addClient(*this);
		}
		else
			serverChannels.insert(std::make_pair(channels[0], Channel(channels[0]))); 	// !exist
		i++;
	}

}
