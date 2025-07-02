#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

void Client::quitCommand(const std::string& command)
{
    std::string quit_message = command.substr(command.find(' ') + 1);

     if (!quit_message.empty() && quit_message[quit_message.length() - 1] == '\n') {
        quit_message = quit_message.substr(0, quit_message.length() - 1);
        if (!quit_message.empty() && quit_message[quit_message.length() - 1] == '\r') {
            quit_message = quit_message.substr(0, quit_message.length() - 1);
        }
    }

    if (!quit_message.empty() && quit_message[0] == ':')
        quit_message = quit_message.substr(1);

    if (quit_message.empty())
        quit_message = "Client Quit";

    std::string nick = _nickname.empty() ? "*" : _nickname;
    std::string user = _username.empty() ? "unknown" : _username;
    std::string host = _hostname.empty() ? "localhost" : _hostname;
    std::string quit_response = ":" + nick + "!" + user + "@" + host + " QUIT :" + quit_message + "\r\n";

    std::map<std::string, Channel>& channels = _server->getChannels();
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
        Channel& channel = it->second;
        std::vector<Client> channel_clients = channel.getClients();

        bool is_in_channel = false;
        for (size_t i = 0; i < channel_clients.size(); ++i) {
            if (channel_clients[i].getSocketFd() == _socket_fd)
			{
                is_in_channel = true;
                break;
            }
        }
        if (is_in_channel)
		{
            for (size_t i = 0; i < channel_clients.size(); ++i)
			{
                if (channel_clients[i].getSocketFd() != _socket_fd)
				{
                    send(channel_clients[i].getSocketFd(), quit_response.c_str(), quit_response.length(), 0);
                }
            }
            channel.removeClient(*this);
            std::cout << "Client " << nick << " removed from channel " << channel.getName() << std::endl;
        }
    }

    std::cout << "Client " << nick << " disconnected with message: " << quit_message << std::endl;
}