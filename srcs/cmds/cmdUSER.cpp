#include "Irc.hpp"

void Client::userCommand(std::string command)
{
	std::string userInfo = command.substr(command.find(' ') + 1);

	if (userInfo.empty())
	{
		std::string response = ":localhost 461 " + this->_nickname + " USER :Not enough parameters\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		return;
	}

	std::stringstream ss(userInfo);
	std::string username, hostname, servername, realname, token;
	std::vector<std::string> params;

	while (ss >> token)
	{
		params.push_back(token);
	}

	if (params.size() < 4)
	{
		std::string response = ":localhost 461 " + this->_nickname + " USER :Not enough parameters\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		return;
	}

	username = params[0];
	hostname = params[1];
	servername = params[2];
	realname = "";
	for (size_t i = 3; i < params.size(); ++i)
	{
		if (i == 3 && params[i][0] != ':')
		{
			std::string response = ":localhost 461 " + this->_nickname + " USER :Invalid format, realname must start with ':'\r\n";
			send(this->_socket_fd, response.c_str(), response.length(), 0);
			return;
		}
		if (i == 3)
			realname = params[i].substr(1);
		else
			realname += " " + params[i];
	}

	if (username.empty() || hostname.empty() || servername.empty() || realname.empty())
	{
		std::string response = ":localhost 461 " + this->_nickname + " USER :Invalid or empty parameters\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		return;
	}

	this->_username = username;
	this->_hostname = hostname;
	this->_servername = servername;
	this->_realname = realname;
	this->_authStep.isUserSet = true;
}
