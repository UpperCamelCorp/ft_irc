#include "Irc.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

void Client::topicCommand(const std::string& command)
{
    std::string params = command.substr(command.find(' ') + 1);

    if (!params.empty() && params[params.length() - 1] == '\n')
	{
        params = params.substr(0, params.length() - 1);
        if (!params.empty() && params[params.length() - 1] == '\r')
		{
            params = params.substr(0, params.length() - 1);
        }
    }

    if (params.empty())
	{
        std::string response = ":localhost 461 " + (_nickname.empty() ? "*" : _nickname) + " TOPIC :Not enough parameters\r\n";
        send(_socket_fd, response.c_str(), response.length(), 0);
        std::cout << "Error: TOPIC command missing parameters for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
        return;
    }

    std::stringstream ss(params);
    std::string channel_name, topic;
    ss >> channel_name;

    if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
	{
        std::string response = ":localhost 403 " + (_nickname.empty() ? "*" : _nickname) + " " + channel_name + " :No such channel\r\n";
        send(_socket_fd, response.c_str(), response.length(), 0);
        std::cout << "Error: Invalid channel name " << channel_name << " for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
        return;
    }

    std::map<std::string, Channel>& channels = _server->getChannels();
	std::map<std::string, Channel>::iterator chan_it = channels.find(channel_name);
	if (chan_it == channels.end())
	{
        std::string response = ":localhost 403 " + (_nickname.empty() ? "*" : _nickname) + " " + channel_name + " :No such channel\r\n";
        send(_socket_fd, response.c_str(), response.length(), 0);
        std::cout << "Error: Channel " << channel_name << " does not exist for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
        return;
    }

    Channel& channel = chan_it->second;

    std::vector<Client> channel_clients = channel.getClients();
    bool is_in_channel = false;
    for (size_t i = 0; i < channel_clients.size(); ++i)
	{
        if (channel_clients[i].getSocketFd() == _socket_fd)
		{
            is_in_channel = true;
            break;
        }
    }
    if (!is_in_channel)
	{
        std::string response = ":localhost 442 " + (_nickname.empty() ? "*" : _nickname) + " " + channel_name + " :You're not on that channel\r\n";
        send(_socket_fd, response.c_str(), response.length(), 0);
        std::cout << "Error: Client " << (_nickname.empty() ? "unregistered" : _nickname) << " not in channel " << channel_name << std::endl;
        return;
    }

    std::string remaining;
    std::getline(ss, remaining);
    if (!remaining.empty() && remaining[0] == ' ')
	{
        remaining = remaining.substr(1);
    }
    if (remaining.empty())
{
        std::string current_topic = channel.getTopic();
        if (current_topic.empty())
		{
            std::string response = ":localhost 331 " + _nickname + " " + channel_name + " :No topic is set\r\n";
            send(_socket_fd, response.c_str(), response.length(), 0);
            std::cout << "Sent RPL_NOTOPIC for channel " << channel_name << " to client " << _nickname << std::endl;
        }
		else
		{
            std::string response = ":localhost 332 " + _nickname + " " + channel_name + " :" + current_topic + "\r\n";
            send(_socket_fd, response.c_str(), response.length(), 0);
            std::cout << "Sent RPL_TOPIC for channel " << channel_name << " to client " << _nickname << std::endl;
        }
        return;
    }

    topic = remaining;
    if (!topic.empty() && topic[0] == ':')
	{
        topic = topic.substr(1);
    }

    if (!channel.isOperator(*this) && channel.getTopicMode())
	{
        std::string response = ":localhost 482 " + _nickname + " " + channel_name + " :You're not channel operator\r\n";
        send(_socket_fd, response.c_str(), response.length(), 0);
        std::cout << "Error: Client " << _nickname << " not operator in channel " << channel_name << std::endl;
        return;
    }

    channel.setTopic(topic);

    std::string nick = _nickname.empty() ? "*" : _nickname;
    std::string user = _username.empty() ? "unknown" : _username;
    std::string host = _hostname.empty() ? "localhost" : _hostname;
    std::string topic_response = ":" + nick + "!" + user + "@" + host + " TOPIC " + channel_name + " :" + (topic.empty() ? "" : topic) + "\r\n";
    for (size_t i = 0; i < channel_clients.size(); ++i)
	{
        send(channel_clients[i].getSocketFd(), topic_response.c_str(), topic_response.length(), 0);
    }

    std::cout << "Client " << nick << " set topic for channel " << channel_name << " to: " << (topic.empty() ? "<empty>" : topic) << std::endl;
}