#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Client.hpp"

#include <iostream>
#include <string>
#include <vector>

class Channel
{
    private:
        std::string _name;
        std::vector<Client> _clients;
        std::string _topic;
        std::vector<int> _operators;
    public:
        Channel(const std::string &name);
        void                addClient(const Client &client);
        void                removeClient(const Client &client);
        void                setTopic(const std::string &topic);
        std::string         getName() const;
        std::vector<Client> getClients() const;
        std::string         getTopic() const;
        void                addOperator(Client &client);
        bool                isOperator(const Client &client) const;
        void                removeOperator(const Client &client);
        std::vector<int>    getOperators() const;
        void                sendMessage(const std::string &message, const Client &sender);
};

#endif