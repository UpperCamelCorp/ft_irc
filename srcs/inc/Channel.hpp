#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Irc.hpp"
#include "Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::vector<Client> _clients;
        std::string _topic;
        std::vector<int> _operators;
        bool _topicMode;
        bool _inviteOnly;
        std::string _key;
    public:
        Channel(const std::string &name);
        void                addClient(const Client &client);
        void                removeClient(const Client &client);
        void                setTopic(const std::string &topic);
        std::string         getName() const;
        std::vector<Client> getClients() const;
        std::string         getTopic() const;
        void                addOperator(const Client &client);
        bool                isOperator(const Client &client) const;
        void                removeOperator(const Client &client);
        std::vector<int>    getOperators() const;
        void                sendMessage(const std::string &message, const Client &sender);
        void                setTopicMode(bool mode);
        bool                getTopicMode() const;
        void                setInviteOnly(bool inviteOnly);
        bool                isInviteOnly() const;
        void                setChannelKey(const std::string &key);
        std::string         getChannelKey() const;
};

#endif