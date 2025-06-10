#include "../inc/Client.hpp"

void Client::userCommand(std::string command)
{
	std::string userInfo = command.substr(command.find(' ') + 1);
	if (userInfo.empty())
	{
		std::string response = ":localhost 461 " + this->_nickname + " USER :Not enough parameters\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		return;
	}
	this->_username = userInfo;
	std::cout << "User information set to: " << this->_username << std::endl;
	this->_authStep.isUserSet = true;
}
