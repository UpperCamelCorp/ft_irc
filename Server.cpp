/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:57:56 by olardeux          #+#    #+#             */
/*   Updated: 2025/05/14 12:58:27 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port) : _port(port)
{
    struct pollfd serverFd;
    int optValue = 1;

    this->_server_addr.sin_family = AF_INET;
    this->_server_addr.sin_addr.s_addr = INADDR_ANY;
    this->_server_addr.sin_port = htons(this->_port);
    
    this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket_fd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(optValue)) < 0)
    
    {
        std::cerr << "Error setting socket options" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);
    }

    if (fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(this->_socket_fd, (struct sockaddr *)&this->_server_addr, sizeof(this->_server_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(this->_socket_fd, SOMAXCONN) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(this->_socket_fd);
        exit(EXIT_FAILURE);
    }

    serverFd.fd = this->_socket_fd;
    serverFd.events = POLLIN;
    serverFd.revents = 0;
    this->_poll_fds.push_back(serverFd);
}

Server::~Server()
{
    close(this->_socket_fd);
    for (size_t i = 0; i < this->_client_sockets.size(); ++i)
    {
        close(this->_client_sockets[i]);
    }
}

void Server::acceptClient()
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    struct pollfd clientPollFd;

    client_fd = accept(this->_socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
    {
        throw std::runtime_error("Error accepting client");
    }

    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
    {
        close(client_fd);
        throw std::runtime_error("Error setting client socket to non-blocking");
    }

    clientPollFd.fd = client_fd;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    this->_poll_fds.push_back(clientPollFd);
    this->_client_sockets.push_back(client_fd);
}

void Server::handleClient(int client_fd)
{
    char buffer[1024];
    ssize_t bytes_received;

    memset(buffer, 0, sizeof(buffer));
    bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0)
    {
        closeClient(client_fd);
        throw std::runtime_error("Error receiving data from client");
    }
    else if (bytes_received == 0)
    {
        std::cout << "Client disconnected" << std::endl;
        closeClient(client_fd);
        return;
    }

    std::cout << "From : " << client_fd << " received: " << buffer << std::endl;
}

void Server::closeClient(int client_fd)
{
    close(client_fd);
    for (size_t i = 0; i < this->_poll_fds.size(); ++i)
    {
        if (this->_poll_fds[i].fd == client_fd)
        {
            this->_poll_fds.erase(this->_poll_fds.begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < this->_client_sockets.size(); ++i)
    {
        if (this->_client_sockets[i] == client_fd)
        {
            this->_client_sockets.erase(this->_client_sockets.begin() + i);
            break;
        }
    }
}


void Server::start()
{
    this->isRunning = 1;
    while (this->isRunning)
    {
        if (poll(this->_poll_fds.data(), this->_poll_fds.size(), -1) < 0)
        {
            std::cerr << "Error polling sockets" << std::endl;
            throw std::runtime_error("Error polling sockets");
        }
        for (size_t i = 0; i < this->_poll_fds.size(); ++i)
        {
            if (this->_poll_fds[i].revents & POLLIN)
            {
                if (this->_poll_fds[i].fd == this->_socket_fd)
                {
                    acceptClient();
                }
                else
                {
                    handleClient(this->_poll_fds[i].fd);
                }
            }
        }
    }
}