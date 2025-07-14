#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"

static void	ErrInvalid(int error_n, std::string err_arg, int socket_fd)
{
    std::string error_msg;  
    if (error_n == 461)
        error_msg = ":localhost 461 " + err_arg + " MODE :Not enough parameters\r\n";
    else if (error_n == 403)
        error_msg = ":localhost 403 " + err_arg + " :No such channel\r\n";
    else if (error_n == 482)
        error_msg = ":localhost 482 " + err_arg + " :You're not channel operator\r\n";
    else if (error_n == 441)
        error_msg = ":localhost 441 " + err_arg + " :They aren't on that channel\r\n";
    else
        return;
    send(socket_fd, error_msg.c_str(), error_msg.length(), 0);
}

void    addChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    switch (mode[0])
    {
        case 't': {
            channel.setTopicMode(true);
            return;
        }
        case 'i': {
            channel.setInviteOnly(true);
            return;
        }
        case 'k':{
            if (value.empty())
            {
                std::cerr << "Key mode requires a key value." << std::endl;
                ErrInvalid(461, channel.getName(), client.getSocketFd());
                return;
            }
            channel.setChannelKey(value);
            return;
        }
        case 'o': {
            if (value.empty())
            {                
                std::cerr << "Operator mode requires a nickname." << std::endl;
                ErrInvalid(461, channel.getName(), client.getSocketFd());
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
            ErrInvalid(441, client.getNick() + " " + value + " " + channel.getName(), client.getSocketFd());
            return;
        }
        case 'l': {
            if (!value.empty())
            {
                long limit = atol(value.c_str());
                if (limit > 0 && limit <= 1000)
                {
                    channel.setMaxClients(limit);
                    return;
                }
            }
            std::cerr << "Limit mode requires a numeric value." << std::endl;
            ErrInvalid(461, channel.getName(), client.getSocketFd());
            return;
        }
    }
}

void removeChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    switch (mode[0])
    {
        case 't': {
            channel.setTopicMode(false);
            return;
        }
        case 'i': {
            channel.setInviteOnly(false);
            return;
        }
        case 'k': {
            channel.setChannelKey("");
            return;
        }
        case 'o': {
            if (value.empty())
            {
                std::cerr << "Operator mode requires a nickname." << std::endl;
                ErrInvalid(461, channel.getName(), client.getSocketFd());
                return;
            }
            if (channel.isOperator(client))
            {
                channel.removeOperator(client);
                return;
            }
            std::cerr << "Client is not an operator in the channel." << std::endl;
            ErrInvalid(482, client.getNick() + " " + channel.getName(), client.getSocketFd());
            return;
        }
        case 'l': {
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
    std::cout << "MODE command received: " << command << std::endl;
    while (std::getline(iss, token, ' '))
    {
        if (!token.empty())
            args.push_back(token);
    }
    if (args.size() < 3)
    {
        std::cerr << "Not enough arguments for MODE command." << std::endl;
        ErrInvalid(461, this->getNick(), this->_socket_fd);
        return;
    }
    if (args[1][0] != '#')
    {
        std::cerr << "Invalid channel name for MODE command." << std::endl;
        ErrInvalid(403, this->getNick(), this->_socket_fd);
        return;
    }
    Channel *channel = this->_server->getChannelByName(args[1]);
    if (!channel)
    {
        std::cerr << "Channel not found for MODE command." << std::endl;
        ErrInvalid(403, this->getNick(), this->_socket_fd);
        return;
    }
    if (!channel->isOperator(*this))
    {
        std::cerr << "Client is not an operator in the channel." << std::endl;
        ErrInvalid(482, this->getNick(), this->_socket_fd);
        return;
    }
    std::string mode = args[2];
    if (mode[0] == '+')
    {
        mode.erase(0, 1);
        if (mode.empty())
        {
            std::cerr << "No mode specified to add." << std::endl;
            ErrInvalid(461, this->getNick(), this->_socket_fd);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            ErrInvalid(461, this->getNick(), this->_socket_fd);
            return;
        }
        addChannelMode(*channel, *this, mode.substr(0, 1), args.size() > 3 ? args[3] : "");
    }
    else if (mode[0] == '-')
    {
        mode.erase(0, 1);
        if (mode.empty())
        {
            std::cerr << "No mode specified to remove." << std::endl;
            ErrInvalid(461, this->getNick(), this->_socket_fd);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            ErrInvalid(461, this->getNick(), this->_socket_fd);
            return;
        }
        removeChannelMode(*channel, *this, mode.substr(0, 1), args.size() > 3 ? args[3] : "");
    }
    else
    {
        std::cerr << "Invalid mode prefix. Use '+' or '-'." << std::endl;
        ErrInvalid(461, this->getNick(), this->_socket_fd);
    }
}