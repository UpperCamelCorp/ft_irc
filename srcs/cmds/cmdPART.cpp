#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Client::partCommand(const std::string& command)
{
	std::string params = command.substr(command.find(' ') + 1);
	if (!params.empty() && params[params.length() - 1] == '\n')
	{
		params = params.substr(0, params.length() - 1);
		if (!params.empty() && params[params.length() - 1] == '\r')
			params = params.substr(0, params.length() - 1);
	}

	if (params.empty())
	{
		std::string response = ":localhost 461 " + (_nickname.empty() ? "*" : _nickname) + " PART :Not enough parameters\r\n";
		send(_socket_fd, response.c_str(), response.length(), 0);
		std::cout << "Error: PART command missing parameters for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
		return;
	}

	std::string channels_str, part_message;
	std::size_t space_pos = params.find(' ');
	if (space_pos != std::string::npos)
	{
		channels_str = params.substr(0, space_pos);
		part_message = params.substr(space_pos + 1);
		if (!part_message.empty() && part_message[0] == ':')
			part_message = part_message.substr(1);
	}
	else
		channels_str = params;

	std::vector<std::string> channels;
	std::stringstream ss(channels_str);
	std::string channel_name;
	while (std::getline(ss, channel_name, ','))
	{
		if (!channel_name.empty())
			channels.push_back(channel_name);
	}

	if (channels.empty())
	{
		std::string response = ":localhost 461 " + (_nickname.empty() ? "*" : _nickname) + " PART :Not enough parameters\r\n";
		send(_socket_fd, response.c_str(), response.length(), 0);
		std::cout << "Error: No valid channels provided for PART by client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
		return;
	}

	for (size_t i = 0; i < channels.size(); ++i)
	{
		channel_name = channels[i];

		if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
		{
			std::string response = ":localhost 403 " + (_nickname.empty() ? "*" : _nickname) + " " + channel_name + " :No such channel\r\n";
			send(_socket_fd, response.c_str(), response.length(), 0);
			std::cout << "Error: Invalid channel name " << channel_name << " for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
			continue;
		}

		std::map<std::string, Channel>& channels = _server->getChannels();
		std::map<std::string, Channel>::iterator chan_it = channels.find(channel_name);
		if (chan_it == channels.end())
		{
			std::string response = ":localhost 403 " + (_nickname.empty() ? "*" : _nickname) + " " + channel_name + " :No such channel\r\n";
			send(_socket_fd, response.c_str(), response.length(), 0);
			std::cout << "Error: Channel " << channel_name << " does not exist for client " << (_nickname.empty() ? "unregistered" : _nickname) << std::endl;
			continue;
		}

		Channel& channel = chan_it->second;

		std::vector<Client> channel_clients = channel.getClients();
		bool is_in_channel = false;
		for (size_t j = 0; j < channel_clients.size(); ++j)
		{
			if (channel_clients[j].getSocketFd() == _socket_fd)
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
			continue;
		}

		std::string nick = _nickname.empty() ? "*" : _nickname;
		std::string user = _username.empty() ? "unknown" : _username;
		std::string host = _hostname.empty() ? "localhost" : _hostname;
		std::string part_response = ":" + nick + "!" + user + "@" + host + " PART " + channel_name;
		if (!part_message.empty())
			part_response += " :" + part_message;
		part_response += "\r\n";

		for (size_t j = 0; j < channel_clients.size(); ++j)
		{
			send(channel_clients[j].getSocketFd(), part_response.c_str(), part_response.length(), 0);
		}

		channel.removeClient(*this);
		std::cout << "Client " << nick << " parted from channel " << channel_name << " with message: " << (part_message.empty() ? "<none>" : part_message) << std::endl;
	}
}