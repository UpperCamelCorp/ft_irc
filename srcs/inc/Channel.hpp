#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Irc.hpp"
#include "Client.hpp"

#include <iostream>
#include <string>
#include <vector>

class Client;
class Channel
{
    private:
        std::string _name;
        std::vector<Client> _clients;
        std::string _topic;
        std::vector<int> _operators;
        std::string _password;
    public:
        Channel(const std::string &name);
        Channel(const std::string &name, const std::string &password);
        bool                addClient(Client &client, std::string password);
        void                addOperator(Client &client);
        void                removeClient(Client &client);
        void                removeOperator(Client &client);
        void                sendMessage(const std::string &message, Client &sender);
        void                setTopic(const std::string &topic);
        void                setPassword(const std::string &password);
        bool                goodPassword(const std::string &password);

        std::string         getTopic() const;
        std::vector<Client> getClients() const;
        std::string         getName() const;
        std::vector<int>    getOperators() const;
        bool                isOperator(Client &client) const;
        std::string         getPassword() const;
};

#endif