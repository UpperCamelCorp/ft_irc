#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"

extern bool isRunning;

class Client;
class Channel;

class Server
{
    private:
        int                        _port;
        std::string                _password;
        int                        _socket_fd;
        struct sockaddr_in         _server_addr;
        std::map<int, Client>      _clients;
        std::map<std::string, Channel> _channels;
        std::vector<struct pollfd> _poll_fds;

        void                       acceptClient();
        void                       handleClient(Client &client);

    public:
        Server(int port);
        ~Server();
        void                            start();
        void                            closeClient(int client_fd);
        void                            setPassword(const std::string &password);
        const std::string&              getPassword() const;
        void 							handleSignal(int signal);
		std::map<std::string, Channel>& getChannels();
        bool                			isNicknameAvailable(const std::string &nickname) const;
        std::map<int, Client>			getClients(void) const;
        Client*                         getClientByNick(const std::string &nick);
        Channel*                        getChannelByName(const std::string &name);
        bool                            isNameDuplicate(const std::string &name) const;
};

#endif