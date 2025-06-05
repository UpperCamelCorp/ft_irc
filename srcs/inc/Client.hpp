/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:16:37 by olardeux          #+#    #+#             */
/*   Updated: 2025/06/05 15:00:39 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

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

class Client
{
    private:
        int         _socket_fd;
        std::string _name;
        std::string _nickname;
        std::string _recvCommand;
        void        ircCommand(std::string command);
    public:
        void setSocketFd(int fd);
        int  getSocketFd() const;
        void handleCommand(std::string command);
};

#endif