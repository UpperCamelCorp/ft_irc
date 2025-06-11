#include "../inc/Client.hpp"

void Client::quitCommand(std::string command)
{
    std::string quit_message = command.substr(command.find(' ') + 1);

    if (!quit_message.empty() && quit_message.back() == '\n') {
        quit_message.pop_back();
        if (!quit_message.empty() && quit_message.back() == '\r') {
            quit_message.pop_back();
        }
    }

    if (!quit_message.empty() && quit_message[0] == ':') {
        quit_message = quit_message.substr(1);
    }

    if (quit_message.empty()) {
        quit_message = "Client Quit";
    }
}