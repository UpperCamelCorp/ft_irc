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
            if (!value.empty()) // Limit mode
            {
                long limit = atol(value.c_str());
                if (limit > 0 && limit <= 1000)
                {
                    channel.setMaxClients(limit);
                    return;
                }
            }
            std::cerr << "Limit mode requires a numeric value." << std::endl;
            std::string error = ":localhost 461 " + channel.getName() + " MODE :Not enough parametersz\r\n";
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return; 
        }
    }
}

void removeChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    switch (mode[0])
    {
        case 't': { // Topic mode
            channel.setTopicMode(false);
            return;
        }
        case 'i': { // Invite-only mode
            channel.setInviteOnly(false);
            return;
        }
        case 'k': { // Key mode
            channel.setChannelKey("");
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
            if (channel.isOperator(client))
            {
                channel.removeOperator(client);
                return;
            }
            std::cerr << "Client is not an operator in the channel." << std::endl;
            std::string error = ":localhost 482 " + client.getNick() + " " + channel.getName() + " :You're not channel operator\r\n";
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return;
        }
        case 'l': { // Limit mode
            channel.setMaxClients(0);
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
    std::cout << args[2] << std::endl;
    Channel *channel = this->_server->getChannelByName(args[1]);
    if (!channel)
    {
        std::cerr << "Channel not found for MODE command." << std::endl;
        error = ":localhost 403 " + this->getNick() + " " + args[1] + " :No such channel\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
        return;
    }
    if (!channel->isOperator(*this))
    {
        std::cerr << "Client is not an operator in the channel." << std::endl;
        error = ":localhost 482 " + this->getNick() + " " + args[1] + " :You're not channel operator\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
        return;
    }
    std::string mode = args[2];
    if (mode[0] == '+')
    {
        mode.erase(0, 1);
        if (mode.empty())
        {
            std::cerr << "No mode specified to add." << std::endl;
            error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
            send(this->_socket_fd, error.c_str(), error.length(), 0);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
            send(this->_socket_fd, error.c_str(), error.length(), 0);
            return;
        }
        addChannelMode(*channel, *this, mode.substr(0, 1), mode.substr(2));
    }
    else if (mode[0] == '-')
    {
        mode.erase(0, 1);
        if (mode.empty())
        {
            std::cerr << "No mode specified to remove." << std::endl;
            error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
            send(this->_socket_fd, error.c_str(), error.length(), 0);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
            send(this->_socket_fd, error.c_str(), error.length(), 0);
            return;
        }
        removeChannelMode(*channel, *this, mode.substr(0, 1), mode.substr(2));
    }
    else
    {
        std::cerr << "Invalid mode prefix. Use '+' or '-'." << std::endl;
        error = ":localhost 461 " + this->getNick() + " MODE :Not enough parameters\r\n";
        send(this->_socket_fd, error.c_str(), error.length(), 0);
    }
}