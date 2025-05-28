/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olardeux <olardeux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:27:39 by olardeux          #+#    #+#             */
/*   Updated: 2025/05/14 12:41:08 by olardeux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Invalid port number" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        Server server(port);
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}