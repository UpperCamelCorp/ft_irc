#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
# include <fcntl.h>

extern bool isRunning;

class Client;
class Channel;

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