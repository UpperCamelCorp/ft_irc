/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 09:30:22 by olardeux          #+#    #+#             */
/*   Updated: 2025/05/14 12:57:16 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <vector>
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
        std::vector<int>           _client_sockets;
        std::vector<struct pollfd> _poll_fds;
        void                       closeClient(int client_fd);
    
    public:
        Server(int port);
        ~Server();
        void                start();
        void                acceptClient();
        void                handleClient(int client_fd);
};

#endif