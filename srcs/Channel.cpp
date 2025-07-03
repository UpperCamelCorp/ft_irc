#include "Channel.hpp"
#include "Irc.hpp"

#include "Client.hpp"

Channel::Channel(const std::string &name) : _name(name), _topic(""), _password("")
{
}

Channel::Channel(const std::string &name, const std::string &password) : _name(name), _password(password) 
{
    std::cout << "Created a restricted channel called : " << name << std::endl;
}
/**
 * @brief Adds a client to the channel.
 *
 * This function appends the given client to the list of clients
 * currently present in the channel.
 *
 * @param client Reference to the Client object to be added.
 */
bool Channel::addClient(Client &client, std::string password)
{
    if (_password == "" || password == _password)
        this->_clients.push_back(client);
    else
        return false;
    return true;
}

/**
 * @brief Removes a client from the channel.
 *
 * Iterates through the list of clients in the channel and removes the client
 * whose socket file descriptor matches that of the provided client.
 *
 * @param client Reference to the client to b *_server; // pointer vers le l'intsance de Server (pour les channels)
        int         _socket_fd;e removed from the channel.
 */
void Channel::removeClient(const Client &client)
{
    for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (it->getSocketFd() == client.getSocketFd())
        {
            this->_clients.erase(it);
            return;
        }
    }
}

/**
 * @brief Retrieves a list of clients currently in the channel.
 *
 * @return A vector containing copies of the Client objects associated with this channel.
 */
std::vector<Client> Channel::getClients() const
{
    return this->_clients;
}

/**
 * @brief Sets the topic of the channel.
 *
 * This function updates the channel's topic to the specified string.
 *
 * @param topic The new topic to set for the channel.
 */
void Channel::setTopic(const std::string &topic)
{
    this->_topic = topic;
}

/**
 * @brief Retrieves the name of the channel.
 *
 * @return The name of the channel as a std::string.
 */
std::string Channel::getName() const
{
    return this->_name;
}

/**
 * @brief Retrieves the current topic of the channel.
 *
 * @return The topic of the channel as a std::string.
 */
std::string Channel::getTopic() const
{
    return this->_topic;
}

/**
 * @brief Adds the given client as an operator of the channel if they are not already an operator.
 *
 * This function checks if the specified client is already an operator in the channel.
 * If not, it adds the client's socket file descriptor to the list of channel operators.
 *
 * @param client Reference to the Client object to be added as an operator.
 */
void Channel::addOperator(Client &client)
{
    if (!isOperator(client))
        this->_operators.push_back(client.getSocketFd());
}

/**
 * @brief Removes the specified client from the list of channel operators.
 *
 * This function searches for the given client's socket file descriptor in the
 * channel's list of operator socket file descriptors. If found, the client is
 * removed from the list of operators.
 *
 * @param client Reference to the Client object to be removed as an operator.
 */
void Channel::removeOperator(const Client &client)
{
    for (std::vector<int>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if (*it == client.getSocketFd())
        {
            this->_operators.erase(it);
            return;
        }
    }
}
/**
 * @brief Retrieves the list of operator IDs  *_server; // pointer vers le l'intsance de Server (pour les channels)
        int         _socket_fd;for the channel.
 *
 * @return A vector containing the IDs of users who are operators in this channel.
 */
std::vector<int> Channel::getOperators() const
{
    return this->_operators;
}

/**
 * @brief Checks if the given client is an operator in the channel.
 *
 * This function iterates through the list of operator socket file descriptors
 * and compares each with the socket file descriptor of the provided client.
 *
 * @param client Reference to the Client object to check.
 * @return true if the client is an operator in the channel, false otherwise.
 */
bool Channel::isOperator(const Client &client) const
{
    for (std::vector<int>::const_iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if (*it == client.getSocketFd())
            return true;
    }
    return false;
}

/**
 * @brief Sends a message to all clients in the channel except the sender.
 *
 * This function iterates through the list of clients in the channel and sends
 * the specified message to each client, excluding the one who sent the message.
 *
 * @param message The message to be sent to other clients. This message must already be formatted according to the IRC RFC specifications.
 * @param sender Reference to the Client object who is sending the message.
 */
void Channel::sendMessage(const std::string &message, const Client &sender)
{
    for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (it->getSocketFd() != sender.getSocketFd())
        {
            send(it->getSocketFd(), message.c_str(), message.length(), 0);
        }
    }
}

/* -- Password get/set ------------------------------------------------------------*/

void            Channel::setPassword(const std::string &password) {
    _password = password;
}

bool            Channel::goodPassword(const std::string &password) {
    if (password == _password)
        return true;
    return false;
}

std::string     Channel::getPassword() const {
    return (_password);
}
