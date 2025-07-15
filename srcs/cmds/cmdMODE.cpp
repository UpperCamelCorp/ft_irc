#include "../inc/Client.hpp"
#include "../inc/Channel.hpp"
#include "../inc/Server.hpp"
#include "../inc/Irc.hpp"
#include "../inc/ErrMacro.hpp"

void    addChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    std::string error;
    std::string rpl;
    switch (mode[0])
    {
        case 't': {
            channel.setTopicMode(true);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " +t\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'i': {
            channel.setInviteOnly(true);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " +i\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'k':{
            if (value.empty())
            {
                std::cerr << "Key mode requires a key value." << std::endl;
                error = ERR_NEEDMOREPARAMS(channel.getName(), "k");
                send(client.getSocketFd(), error.c_str(), error.length(), 0);
                return;
            }
            channel.setChannelKey(value);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " +k " + value + "\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'o': {
            if (value.empty())
            {                
                std::cerr << "Operator mode requires a nickname." << std::endl;
                error = ERR_NEEDMOREPARAMS(channel.getName(), "o");
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
                        rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " +o " + value + "\r\n";
                        send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
                        channel.sendMessage(rpl, client);
                        return;
                    }
                }
            }
            std::cerr << "No such user in channel for operator mode." << std::endl;
            error = ERR_NOTONCHANNEL(client.getNick(), channel.getName());
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return;
        }
        case 'l': {
            if (!value.empty())
            {
                long limit = atol(value.c_str());
                if (limit > 0 && limit <= 1000)
                {
                    channel.setMaxClients(limit);
                    rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " +l " + value + "\r\n";
                    send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
                    channel.sendMessage(rpl, client);
                    return;
                }
            }
            std::cerr << "Limit mode requires a numeric value." << std::endl;
            error = ERR_NEEDMOREPARAMS(channel.getName(), "l");
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return;
        }
    }
}

void removeChannelMode(Channel &channel, Client &client, const std::string &mode, const std::string &value)
{
    std::string error;
    std::string rpl;
    switch (mode[0])
    {
        case 't': {
            channel.setTopicMode(false);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " -t\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'i': {
            channel.setInviteOnly(false);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " -i\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'k': {
            channel.setChannelKey("");
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " -k\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
        case 'o': {
            if (value.empty())
            {
                std::cerr << "Operator mode requires a nickname." << std::endl;
                error = ERR_NEEDMOREPARAMS(channel.getName(), "o");
                send(client.getSocketFd(), error.c_str(), error.length(), 0);
                return;
            }
            std::vector<Client> clients = channel.getClients();
            for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                if (it->getNick() == value)
                {
                    if (channel.isOperator(*it) && it->getNick() != client.getNick())
                    {
                        channel.removeOperator(*it);
                        rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " -o " + value + "\r\n";
                        send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
                        channel.sendMessage(rpl, client);
                        return;
                    }
                    if (it->getNick() == client.getNick())
                    {
                        error = ERR_CANNOTREMOVEOP(client.getNick(), channel.getName());
                    }
                }
            }
            std::cerr << "Client is not an operator in the channel." << std::endl;
            error = ERR_NOTONCHANNEL(client.getNick(), channel.getName());
            send(client.getSocketFd(), error.c_str(), error.length(), 0);
            return;
        }
        case 'l': {
            channel.setMaxClients(-1);
            rpl = ":" + client.getNick() + "!" + client.getUser() + "@" + "localhost" + " MODE " + channel.getName() + " -l\r\n";
            send(client.getSocketFd(), rpl.c_str(), rpl.length(), 0);
            channel.sendMessage(rpl, client);
            return;
        }
    }
}

void Client::modeCommand(const std::string &command)
{
    std::vector<std::string> args = split_cmd(command, ' ');
    std::string response;
    if (args.size() < 2)
    {
        std::cerr << "Not enough arguments for MODE command." << std::endl;
        response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    if (!valid_channel_name(args[1]))
    {
        if (this->_nickname == args[1])
            return;
        std::cerr << "Invalid channel name for MODE command." << std::endl;
        response = ERR_NOSUCHCHANNEL(this->getNick(), args[1]);
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    Channel *channel = this->_server->getChannelByName(args[1]);
    if (!channel)
    {
        std::cerr << "Channel not found for MODE command." << std::endl;
        response = ERR_NOSUCHCHANNEL(this->getNick(), args[1]);
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    if (args.size() == 2)
    {
        std::string modes = channel->getMode();
        std::string mode_string = "MODE " + channel->getName() + " :" + modes + "\r\n";
        send(this->_socket_fd, mode_string.c_str(), mode_string.length(), 0);
        return;
    }
    if (!channel->isOperator(*this))
    {
        std::cerr << "Client is not an operator in the channel." << std::endl;
        response = ERR_CHANOPRIVSNEEDED(this->getNick(), channel->getName());
        send(this->_socket_fd, response.c_str(), response.length(), 0);
        return;
    }
    std::string mode = args[2];
    if (mode[0] == '+')
    {
        mode.erase(0, 1);
        if (mode.empty())
        {
            std::cerr << "No mode specified to add." << std::endl;
            response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
            send(this->_socket_fd, response.c_str(), response.length(), 0);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
            send(this->_socket_fd, response.c_str(), response.length(), 0);
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
            response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
            send(this->_socket_fd, response.c_str(), response.length(), 0);
            return;
        }
        if (mode.length() > 1 && mode[1] == ' ')
        {
            std::cerr << "Invalid mode format." << std::endl;
            response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
            send(this->_socket_fd, response.c_str(), response.length(), 0);
            return;
        }
        removeChannelMode(*channel, *this, mode.substr(0, 1), args.size() > 3 ? args[3] : "");
    }
    else
    {
        std::cerr << "Invalid mode prefix. Use '+' or '-'." << std::endl;
        response = ERR_NEEDMOREPARAMS(this->getNick(), "MODE");
        send(this->_socket_fd, response.c_str(), response.length(), 0);
    }
}