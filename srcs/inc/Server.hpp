#ifndef SERVER_HPP
# define SERVER_HPP

#include "Irc.hpp"

extern bool isRunning;

class Server
{
    private:
        int                        _port;
        int                        _socket_fd;
        struct sockaddr_in         _server_addr;
        std::map<int, Client>      _clients;
        std::map<std::string, Channel> _channels;
        std::vector<struct pollfd> _poll_fds;
        void                       closeClient(int client_fd);
        void                       acceptClient();
        void                       handleClient(Client &client);
    public:
        Server(int port);
        ~Server();
        void                start();
        void handleSignal(int signal);
		std::map<std::string, Channel>& getChannels();
};

#endif