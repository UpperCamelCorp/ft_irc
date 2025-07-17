#include "../inc/Client.hpp"
#include "Server.hpp"
#include "ErrMacro.hpp"

static void	sendError(std::string macro, int socket_fd)
{
	send(socket_fd, macro.c_str(), macro.length(), 0);
}

static int	client_exist(const std::map<int, Client>& list, const std::string target){
	std::map<int, Client>::const_iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it->second.getNick() == target)
		    return it->second.getSocketFd();
	}
	return -1;
}

void	Client::privmsgCommand(const std::string& command)
{
	std::vector<std::string> args;
	args = split_cmd(command, ':');

	if (args.size() < 2)
		return sendError(ERR_NOTEXTTOSEND(this->getNick()), this->getSocketFd());

	int pos;
	pos = command.find(':');
	std::string text_to_send = command.substr(pos);

	std::vector<std::string> target;
	target = split_cmd(args[0], ' ');

	if (target.size() < 2)
		return sendError(ERR_NORECIPIENT(this->getNick()), this->getSocketFd());

	std::string receiver = target[1];

	if (receiver[0] == '#')
	{
		std::map<std::string, Channel>& serverChannels = this->_server->getChannels();
		std::map<std::string, Channel>::iterator it;
		if (serverChannels.find(receiver) != serverChannels.end())
		{
			it = serverChannels.find(receiver);
			std::string builded = RPL_PRIVMSG(this->getNick(), this->_username, receiver, text_to_send);
			it->second.sendMessage(builded, *this);
		}
		else
			return sendError(ERR_NOSUCHCHANNEL(this->getNick(), receiver.c_str()), this->getSocketFd());
	}
	else
	{
		std::map<int, Client> serverClients = this->_server->getClients();
		int target_socket = client_exist(serverClients, receiver);
		if (target_socket != -1)
		{
			std::string builded = RPL_PRIVMSG(this->getNick(), this->_username, receiver, text_to_send);
			send(target_socket, builded.c_str(), builded.length(), 0);
		}
		else
			sendError(ERR_NOSUCHNICK(this->getNick(), receiver), this->_socket_fd);
		return ;
	}
}
