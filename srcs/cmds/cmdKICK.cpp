#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Client::kickCommand(const std::string& command)
{
    std::vector<std::string> args = split_cmd(command, ' ');
    if (args.size() < 3)
	{
        std::string response = ":localhost 461 " + this->getNick() + " KICK :Not enough parameters\r\n";
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    std::string channel_name = args[1];
    std::string target_nick = args[2];
    std::string reason = (args.size() > 3) ? args[3] : "Kicked";

    std::map<std::string, Channel>& channels = this->_server->getChannels();
    std::map<std::string, Channel>::iterator chan_it = channels.find(channel_name);
    if (chan_it == channels.end())
	{
        std::string response = ":localhost 403 " + this->getNick() + " " + channel_name + " :No such channel\r\n";
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    Channel& channel = chan_it->second;
    if (!channel.isOperator(*this))
	{
        std::string response = ":localhost 482 " + this->getNick() + " " + channel_name + " :You're not channel operator\r\n";
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    std::vector<Client> clients = channel.getClients();
    Client* target = NULL;
    for (size_t i = 0; i < clients.size(); ++i)
	{
        if (clients[i].getNick() == target_nick) {
            target = &clients[i];
            break;
        }
    }
    if (!target)
	{
        std::string response = ":" + this->getNick() + "!user@localhost PRIVMSG " + channel_name + " :User not in the channel\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		return;
    }
    if (target->getSocketFd() == this->_socket_fd)
	{
        std::string response = ":" + this->getNick() + "!user@localhost PRIVMSG " + channel_name + " :You cannot kick yourself\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }

    std::string kick_msg = ":" + this->getNick() + " KICK " + channel_name + " " + target_nick + " :" + reason + "\r\n";
    channel.sendMessage(kick_msg, *this);
	send(this->_socket_fd, kick_msg.c_str(), kick_msg.length(), 0);
	send(target->getSocketFd(), kick_msg.c_str(), kick_msg.length(), 0);
	channel.removeClient(*target);



}