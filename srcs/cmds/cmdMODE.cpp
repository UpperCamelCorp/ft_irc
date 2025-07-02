#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

void    addChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    switch (mode[0])
    {
        case 't': { // Topic mode
            channel.setTopicMode(true);
            return;
        }
        case 'i': { // Invite-only mode
            channel.setInviteOnly(true);
            return;
        }
        case 'k':{ // Key mode
            if (value.empty())
            {
                std::cerr << "Key mode requires a key value." << std::endl;
                std::string error = ":localhost 461 " + channel.getName() + " MODE :Not enough parameters\r\n";
                send(client.getSocketFd(), error.c_str(), error.length(), 0);
                return;
            }
            channel.setChannelKey(value);
            return;
        }
        case 'o': { // Operator mode
            if (value.empty())
            {                
                std::cerr << "Operator mode requires a nickname." << std::endl;
                std::string error = ":localhost 461 " + channel.getName() + " MODE :Not enough parameters\r\n";
                send(client.getSocketFd(), error.c_str(), error.length(), 0);
                return;
            }
            std::vector<Client> clients = channel.getClients();
            for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->getNick() == value)
                {
                    if (!channel.isOperator(*it))
                    {
                        channel.addOperator(*it);
                        return;
                    }
                }
            }
            std::cerr << "No such user in channel for operator mode." << std::endl;
            std::string error = ":localhost 441 " + client.getNick() + " " + value + " " + channel.getName() + " :They aren't on that channel\r\n";
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return;
        }
        case 'l': {
            if (!value.empty() && std::all_of(value.begin(), value.end(), ::isdigit)) // Limit mode
            {
                int limit = atoi(value.c_str());
                if (limit > 0)
                {
                    channel.setMaxClients(limit);
                    return;
                }
            }
            std::cerr << "Limit mode requires a numeric value." << std::endl;
            std::string error = ":localhost 461 " + channel.getName() + " MODE :Not enough parameters\r\n";
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return; 
        }
    }
}

void Client::modeCommand(const std::string &command)
{
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string token;
    std::string error;
    while (std::getline(iss, token, ' '))
    {
        if (!token.empty())
            args.push_back(token);
    }
    if (args.size() < 3)
    {
        std::cerr << "Not enough arguments for MODE command." << std::endl;
        error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
        return;
    }
    if (args[1][0] != '#')
    {
        std::cerr << "Invalid channel name for MODE command." << std::endl;
        error = ":localhost 403 " + this->getNick() + " " + args[1] + " :No such channel\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
        return;
    }

    Channel *channel = this->_server->getChannelByName(args[1]);
    if (!channel)
    {
        std::cerr << "Channel not found for MODE command." << std::endl;
        error = ":localhost 403 " + this->getNick() + " " + args[1] + " :No such channel\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
        return;
    }
    std::string mode = args[2];

}