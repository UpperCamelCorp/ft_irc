#include "Irc.hpp"
#include "Client.hpp"

void Client::pingCommand(const std::string& command)
{
	std::string token = command.substr(command.find(' ') + 1);

	token = trim_cmd(token);
	if (token.empty()) {
		std::string response = ":localhost 461 " + (this->_nickname.empty() ? "*" : this->_nickname) + " PING :Not enough parameters\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		std::cout << "Error: PING command missing parameter for client " << (this->_nickname.empty() ? "unregistered" : this->_nickname) << std::endl;
		return;
	}

	std::string pong_token = token;
	if (!pong_token.empty() && pong_token[0] == ':') {
		pong_token = pong_token.substr(1);
	}

	std::string response = "PONG :" + pong_token + "\r\n";
	send(this->_socket_fd, response.c_str(), response.length(), 0);
	std::cout << "PONG sent in response to PING with token: " << pong_token << " for client " << (this->_nickname.empty() ? "unregistered" : this->_nickname) << std::endl;
}
