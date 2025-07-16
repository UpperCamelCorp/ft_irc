#include "Irc.hpp"
#include "Server.hpp"

bool isRunning = true;

void signalHandler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        std::cout << std::endl << "Server shutting down..." << std::endl;
        isRunning = false;
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port>"  << " <password>"<< std::endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Invalid port number" << std::endl;
        std::cerr << "Enter a port number between 1 and 65535." << std::endl;
        return EXIT_FAILURE;
    }
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    try
    {
        Server server(port);
        std::string password(argv[2]);
        server.setPassword(password);
        std::cout << "Server password set to: " << password << std::endl;
        std::cout << "Server started on port " << port << std::endl;
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}