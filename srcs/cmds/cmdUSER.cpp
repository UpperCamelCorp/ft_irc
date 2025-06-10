#include "../inc/Client.hpp"

void Client::userCommand(std::string command)
{
    std::string userInfo = command.substr(command.find(' ') + 1);
    if (userInfo.empty())
    {
        std::cout << "Error: USER command requires user information." << std::endl;
        return;
    }
    this->_name = userInfo;
    std::cout << "User information set to: " << this->_name << std::endl;
    this->_authStep.isUserSet = true;
}

