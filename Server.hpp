/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:30:22 by olardeux          #+#    #+#             */
/*   Updated: 2025/06/04 11:38:56 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"

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


class Server
{
    private:
        int                        _port;
        int                        _socket_fd;
        int                        isRunning;
        struct sockaddr_in         _server_addr;
        std::map<int, Client>          _clients;
        std::vector<struct pollfd> _poll_fds;
        void                       closeClient(int client_fd);
        void                       acceptClient();
        void                       handleClient(Client &client);
    public:
        Server(int port);
        ~Server();
        void                start();
        void handleSignal(int signal);

};

#endif