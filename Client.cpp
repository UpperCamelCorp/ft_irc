/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:10:17 by olardeux          #+#    #+#             */
/*   Updated: 2025/06/04 12:52:19 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void Client::setSocketFd(int fd)
{
    this->_socket_fd = fd;
}

int Client::getSocketFd() const
{
    return this->_socket_fd;
}


void Client::handleCommand(std::string command)
{
    std::string execCommand;
    do
    {
        std::size_t pos = command.find("\r\n");
        if (pos == std::string::npos)
        {
            this->_recvCommand += command;
            return;
        }
        else if (pos == command.length() - 2)
        {
            if (this->_recvCommand.empty())
                execCommand = command.substr(0, pos);
            else
            {
                execCommand = this->_recvCommand + command;
                this->_recvCommand.clear();
            }
            command = command.empty();
        }
        else if (pos != command.length() - 2)
        {
            if (this->_recvCommand.empty())
                execCommand = command.substr(0, pos);
            else
            {
                execCommand = this->_recvCommand + command.substr(0, pos + 2);
                this->_recvCommand.clear();
            }
            command = command.substr(pos + 2);
        }
        std::cout << "Executing command: " << execCommand << std::endl;
        //parsing
    } while (command.length() > 0);
}    
