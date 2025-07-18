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
        std::vector<Client> _invitedClients;
        std::string _topic;
        std::vector<int> _operators;
        std::string _key;
        bool _topicMode;
        bool _inviteOnly;
        int _maxClients;
    public:
        Channel(const std::string &name);
        Channel(const std::string &name, const std::string &password);
        bool                addClient(Client &client, std::string password);
        void                addOperator(Client &client);
        void                removeClient(const Client &client);
        void                removeOperator(const Client &client);
        void                sendMessage(const std::string &message, const Client &sender);
        void                setTopicMode(bool mode);
        bool                getTopicMode() const;
        void                setInviteOnly(bool inviteOnly);
        bool                isInviteOnly() const;
        void                setChannelKey(const std::string &key);
        std::string         getChannelKey() const;
        void                setMaxClients(int maxClients);
        int                 getMaxClients() const;
        int                 getClientCount() const;
        void                setTopic(const std::string &topic);
        void                setPassword(const std::string &password);
        bool                goodPassword(const std::string &password);

        std::string         getTopic() const;
        std::vector<Client> getClients() const;
        std::string         getName() const;
        std::vector<int>    getOperators() const;
        std::vector<Client> getInvitedClients() const;
        bool                isClientInvited(const Client &client) const;
        void                addInvitedClient(const Client &client);
        void                removeInvitedClient(const Client &client);
        bool                isClientInChannel(const Client &client) const;
        bool                isOperator(const Client &client) const;
        std::string         getPassword() const;
        std::string         getMode() const;
};

#endif