#include "../inc/Client.hpp"
#include "Server.hpp"
#include "ErrMacro.hpp"

static void	sendError(std::string macro, int socket_fd)
{
	send(socket_fd, macro.c_str(), macro.length(), 0);
}

static bool	client_exist(const std::map<int, Client>& list, const std::string target){
	std::map<int, Client>::const_iterator it;
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it->second.getNick() == target)
		    return true;
	}
	return false;
}

void	Client::privmsgCommand(const std::string& command)
{
	std::vector<std::string> args;
	args = split_cmd(command, ':');

	if (args.size() < 2)
		return sendError(ERR_NOTEXTTOSEND(this->getNick()), this->getSocketFd());

	std::string text_to_send = args[1];

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
		
		/* send to a channel */
	}
	else
	{
		std::map<int, Client> serverClients = this->_server->getClients();
		if (client_exist(serverClients, receiver))
			std::cout << "Envoie lui le msg\n";
		// for(std::map<std::string, Client>::iterator it ; )
		// ERR_NOSUCHNICK("localhost", receiver.c_str());
		/* send it to a guy */
	}

}