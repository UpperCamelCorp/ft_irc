/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:16:37 by olardeux          #+#    #+#             */
/*   Updated: 2025/06/04 12:24:57 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <unistd.h>

class Client
{
    private:
        int         _socket_fd;
        std::string _name;
        std::string _recvCommand;
        void        _closeClient();
    public:
        void setSocketFd(int fd);
        int  getSocketFd() const;
        void handleCommand(std::string command);
};

#endif