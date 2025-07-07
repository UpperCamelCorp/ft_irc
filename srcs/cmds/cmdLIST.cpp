#include "Server.hpp"

static void	build_msg(const std::string& nick, int socket_fd, const Channel& actual)
{
	std::string response;
	std::ostringstream connected;
	connected << actual.getClients().size();
	
	response = ":localhost 322 " + nick + " " + actual.getName() + " " + connected.str() + " :" + actual.getTopic() + "\n";
	send(socket_fd, response.c_str(), response.length(), 0);
}
static void	RPL(int reply, const std::string& err_arg, int socket_fd)
{
	std::string response;
	if (reply == 321)
		response = ":localhost 321 " + err_arg + " Channel :Users Name\n";
	else if (reply == 323)
		response = ":localhost 323 " + err_arg + " :End of /LIST\n";
	else
		return ;
	send(socket_fd, response.c_str(), response.length(), 0);
}

bool	found_client(const std::vector<Client>& list, const Client& actual){
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i].getNick() == actual.getNick())
			return true;
	}
	return false;
}

static void more_than(int nbclient, Client clientt, std::map<std::string, Channel> servChan){
	RPL(321, clientt.getNick(), clientt.getSocketFd());
	for (std::map<std::string, Channel>::iterator it = servChan.begin()
		; it != servChan.end(); ++it) {
		if (it->second.getClients().size() > static_cast<std::size_t>(nbclient))
			build_msg(clientt.getNick(), clientt.getSocketFd(), it->second);
	}
	RPL(323, clientt.getNick(), clientt.getSocketFd());
}

void	Client::listCommand(const std::string& command) {
	std::vector<std::string> cmd_segment;
	cmd_segment = split_cmd(command, ' ');
	std::map<std::string, Channel>& serverChannels = this->_server->getChannels();
	if (cmd_segment.size() < 2)
	{
		RPL(321, this->getNick(), this->_socket_fd);

		for (std::map<std::string, Channel>::iterator it = serverChannels.begin()
			; it != serverChannels.end(); ++it){

			if (found_client(it->second.getClients(), *this))
				build_msg(this->getNick(), this->_socket_fd, it->second);
			else if (it->second.getPassword() == "")
				build_msg(this->getNick(), this->_socket_fd, it->second);				
			}
		RPL(323, this->getNick(), this->_socket_fd);
	}
	else
	{
		std::vector<std::string> args;
		args = split_cmd(cmd_segment[1], ',');
		if (args.size() < 2)
		{
			if (args[0][0] == '>')
			{
				std::string number_str = args[0].substr(1);
				int value = std::atoi(number_str.c_str());
				more_than(value, *this, serverChannels);
			}
		}
		else
			return ;
	}
}
