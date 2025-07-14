#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/Irc.hpp"
#include "../inc/ErrMacro.hpp"


void Client::passCommand(const std::string& command) {
    std::vector<std::string> args = split_cmd(command, ' ');
    std::string errorMessage;
    if (args.size() < 2) {
        errorMessage = ERR_NEEDMOREPARAMS(_nickname, "PASS");
        send(_socket_fd, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    if (_authStep.isPasswordSet) {
        errorMessage = ERR_ALREADYREGISTERED(_nickname);
        send(_socket_fd, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    std::string password = args[1];
    if (password != _server->getPassword()) {
        errorMessage = ERR_PASSWDMISMATCH(_nickname);
        send(_socket_fd, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }
    _authStep.isPasswordSet = true;
}
