#include "inc/Client.hpp"

#include "Server.hpp"

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
				execCommand = this->_recvCommand + command;
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
		std::cout << "Executing command: " << execCommand << std::endl;
		ircCommand(execCommand);
	} while (command.length() > 0);
	if (!this->_authStep.isRegistered)
		authClient();
	std::cout << "Command handled successfully." << std::endl;
}

void Client::authClient()
{
	std::cout << "Authenticating client: " << this->getNick() << std::endl;
	if (this->_authStep.isNickSet && this->_authStep.isUserSet && !this->_authStep.isRegistered)
	{
		std::string response = ":localhost 001 " + this->getNick() + " :Welcome to the IRC server, " + this->getNick() + "!\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
		this->_authStep.isRegistered = true;
		std::cout << "Client " << this->getNick() << " is now registered." << std::endl;
	}
	else
	{
		std::cout << "Client is not fully authenticated yet." << std::endl;
		if (!this->_authStep.isNickSet)
			std::cout << "Nickname is not set." << std::endl;
		if (!this->_authStep.isUserSet)
			std::cout << "User information is not set." << std::endl;
		std::string response = ":localhost 451 " + this->getNick() + " :You must set your nickname and user information before registering.\r\n";
		send(this->_socket_fd, response.c_str(), response.length(), 0);
	}
}

void Client::unavailableCommand(std::string command)
{
	std::cout << "Error: Command '" << command << "' is not available." << std::endl;
}
void Client::ircCommand(std::string command)
{
    std::string enumtypes[] = {
        "NICK",
        "USER",
        "JOIN",
        "PART",
        "PRIVMSG",
        "PING",
        "PONG",
        "QUIT",
        "LIST",
        "TOPIC",
        "MODE"
    };
    void (Client::*commandFunctions[])(std::string) = {
        &Client::nickCommand,
        &Client::userCommand,
        &Client::joinCommand, // JOIN
        &Client::partCommand,
        &Client::unavailableCommand, // PRIVMSG
        &Client::pingCommand,
        &Client::unavailableCommand, // PONG
        &Client::quitCommand,
        &Client::unavailableCommand, // LIST
        &Client::topicCommand,
        &Client::unavailableCommand  // MODE
    };
    std::string commandType = command.substr(0, command.find(' '));
    for (size_t i = 0; i < 11; ++i)
    {
        if (commandType == enumtypes[i])
        {
            std::cout << "Command type: " << enumtypes[i] << std::endl;
            (this->*commandFunctions[i])(command);
            return;
        }
    }
}
/* -- Getter / Setter ---------------------------------------------------------------*/

std::string     Client::getNick(){
	if (this->_nickname != "")
		return (this->_nickname);
	else
		return ("*");
}
