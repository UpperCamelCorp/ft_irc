#include "Client.hpp"
#include "Irc.hpp"
#include "Server.hpp"
#include "ErrMacro.hpp"

Client::Client() : _server(NULL), _username(""), _hostname(""), _servername(""), _realname(""), _nickname(""), _recvCommand("")
{
	this->_authStep.isNickSet = false;
	this->_authStep.isUserSet = false;
	this->_authStep.isRegistered = false;
	this->_authStep.isPasswordSet = false;
}
void Client::setServer(Server &server)
{
    this->_server = &server;
	if (!this->_server->getPassword().empty())
		this->_authStep.isPasswordSet = false;
	else
		this->_authStep.isPasswordSet = true;
}

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
				execCommand = this->_recvCommand + command.substr(0, pos);
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
		if (!command[0])
			command = command.substr(1);
		ircCommand(execCommand);
	} while (command.length() > 0);
}

void Client::authClient()
{
	std::string response;
	std::cout << "Authenticating client: " << this->getNick() << std::endl;
	if (this->_authStep.isNickSet && this->_authStep.isUserSet && this->_authStep.isPasswordSet && !this->_authStep.isRegistered)
	{
		response = ":localhost 001 " + this->getNick() + " :Welcome to the IRC server, " + this->getNick() + "!\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		this->_authStep.isRegistered = true;
		std::cout << "Client " << this->getNick() << " is now registered." << std::endl;
	}
	else if (this->_authStep.isNickSet && this->_authStep.isUserSet && !this->_authStep.isPasswordSet)
	{
		response = ERR_PASSWDMISMATCH(this->getNick());
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		this->_server->closeClient(this->getSocketFd());
		std::cout << "Client disconnected" << std::endl;
	}
	else
	{
		std::cout << "Client is not fully authenticated yet." << std::endl;
		if (!this->_authStep.isNickSet)
		{
			std::cout << "Nickname is not set." << std::endl;
			return;
		}
		if (!this->_authStep.isUserSet)
			std::cout << "User information is not set." << std::endl;
		response = ":localhost 451 " + this->getNick() + " :You must set your nickname and user information before registering.\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
	}
}

void Client::unavailableCommand(const std::string& command)
{
	if (command == "CAP LS")
		return ;
	send(_socket_fd, ERR_UNKNOWNCOMMAND(_nickname, command).c_str(), ERR_UNKNOWNCOMMAND(_nickname, command).length(), 0);
	std::cout << "Error: Command '" << command << "' is not available." << std::endl;
}
void Client::ircCommand(const std::string& command)
{
    std::string enumtypes[] = {
        "NICK",
        "USER",
		"PASS",
        "JOIN",
        "PART",
        "PRIVMSG",
        "PING",
        "INVITE",
        "QUIT",
        "LIST",
        "TOPIC",
        "MODE",
		"KICK"
    };
    void (Client::*commandFunctions[])(const std::string&) = {
        &Client::nickCommand,
        &Client::userCommand,
        &Client::passCommand,
        &Client::joinCommand,
        &Client::partCommand,
        &Client::privmsgCommand,
        &Client::pingCommand,
		&Client::inviteCommand,
        &Client::quitCommand,
        &Client::listCommand,
        &Client::topicCommand,
        &Client::modeCommand,
		&Client::kickCommand
    };
    std::string commandType = command.substr(0, command.find(' '));
    for (size_t i = 0; i < 13; ++i)
    {
        if (commandType == enumtypes[i])
        {
			if (i > 2 && !this->_authStep.isRegistered)
			{
				std::cout << "Client is not registered, cannot execute command: " << enumtypes[i] << std::endl;
				std::string response = ERR_NOTREGISTERED(this->getNick());
				send(this->_socket_fd, response.c_str(), response.length(), 0);
				return;
			}
            (this->*commandFunctions[i])(command);
            return;
        }
    }
	unavailableCommand(command);
}
/* -- Getter / Setter ---------------------------------------------------------------*/

std::string     Client::getNick() const
{
	if (this->_nickname != "")
		return (this->_nickname);
	else
		return ("*");
}

const std::string&		Client::getUser() const
{
	return (this->_username);
}