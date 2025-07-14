#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

static void	ErrInvalid(int error_n, const std::string &err_arg, int socket_fd)
{
	std::string response;
	if (error_n == 461)
		response = ":localhost 461 " + err_arg + " :No enough parameters\n";
    else if (error_n == 401)
        response = ":localhost 401 " + err_arg + " :No such nick\n";
	else if (error_n == 403)
		response = ":localhost 403 " + err_arg + " :No such channel\n";
    else if (error_n == 443)
        response = ":localhost 443 " + err_arg + " :User already in channel\n";
	else if (error_n == 476)
		response = ":localhost 476 " + err_arg + " :Bad Channel Mask\n";
    else if (error_n == 482)
        response = ":localhost 482 " + err_arg + " :You're not channel operator\n";
    else if (error_n == 442)
        response = ":localhost 442 " + err_arg + " :You're not on that channel\n";
	else
		return ;
	send(socket_fd, response.c_str(), response.length(), 0);
}

void    Client::inviteCommand(const std::string& command)
{
    std::vector<std::string> arglist = split_cmd(command, ' ');
    if (arglist.size() < 3)
    {
        ErrInvalid(461, this->getNick(), this->_socket_fd);
        return;
    }
    std::string channelName = arglist[1];
    std::string nickToInvite = arglist[2];

    if (!valid_channel_name(channelName))
    {
        ErrInvalid(476, channelName, this->_socket_fd);
        return;
    }

    std::map<std::string, Channel>& serverChannels = this->_server->getChannels();
    std::map<std::string, Channel>::iterator it = serverChannels.find(channelName);

    if (it == serverChannels.end())
    {
        ErrInvalid(403, channelName, this->_socket_fd);
        return;
    }
    if (!it->second.isClientInChannel(*this))
    {
        ErrInvalid(442, channelName, this->_socket_fd);
        return;
    }
    if (it->second.isInviteOnly() && !it->second.isOperator(*this))
    {
        ErrInvalid(482, channelName, this->_socket_fd);
        return;
    }
    Channel& channel = it->second;

    Client* clientToInvite = this->_server->getClientByNick(nickToInvite);
    if (!clientToInvite)
    {
        ErrInvalid(401, nickToInvite, this->_socket_fd);
        return;
    }
    if (channel.isOperator(*this))
    {
        std::vector<Client> clientsInChannel = channel.getInvitedClients();
        if (channel.isClientInChannel(*clientToInvite))
        {
            ErrInvalid(443, nickToInvite, this->_socket_fd);
            return;
        }
        else
        {
            channel.addInvitedClient(*clientToInvite);
            std::string response = ":localhost 341 " + this->getNick() + " " + nickToInvite + " " + channelName + "\n";
            send(this->_socket_fd, response.c_str(), response.length(), 0);
        }
    }
    else
        ErrInvalid(482, channelName, this->_socket_fd);
}
