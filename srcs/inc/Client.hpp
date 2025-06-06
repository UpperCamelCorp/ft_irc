/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:16:37 by olardeux          #+#    #+#             */
/*   Updated: 2025/06/06 12:37:17 by olardeux         ###   ########.fr       */
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


typedef struct s_authenticationStep
{
    bool        isNickSet;
    bool        isUserSet;
    bool        isRegistered;
    bool        isPasswordSet;
} t_authStep;


class Client
{
    private:
        int         _socket_fd;
        std::string _name;
        std::string _nickname;
        t_authStep  _authStep;
        std::string _recvCommand;
        void        ircCommand(std::string command);
        void        nickCommand(std::string command);
        void        userCommand(std::string command);
        void        pingCommand(std::string command);
        void       unavailableCommand(std::string command);
        void        authClient();
    public:
        Client();
        void setSocketFd(int fd);
        int  getSocketFd() const;
        void handleCommand(std::string command);
};

#endif