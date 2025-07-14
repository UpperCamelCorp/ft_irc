#include "Irc.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ErrMacro.hpp"

static void	ErrInvalid(int error_n, std::string err_arg, int socket_fd)
{
	std::string response;
	if (error_n == 431)
	{
		response = ":localhost 431 " + err_arg + ":No nickname given\n";
		send(socket_fd, response.c_str(), response.length(), 0);
	}
	else if (error_n == 432)
	{
		response = ":localhost 432 " + err_arg + ":Erroneus nickname\n";
		send(socket_fd, response.c_str(), response.length(), 0);
	}
	else if (error_n == 433)
	{
		response = ":localhost :Nickname is already in use";
		send(socket_fd, response.c_str(), response.length(), 0);
	}
	else
		return ;
}

static bool	valid_charset(std::string nick, std::string actual, int socket_fd)
{
	int	i;

	i = 0;
	while (nick[i])
	{
		if (isalnum(nick[i]) || nick[i] == '{' || nick[i] == '}'
			|| nick[i] == '[' || nick[i] == ']' || nick[i] == '\\'
			|| nick[i] == '|' || (i > 0 && nick[i] == '\n'))
			i++;
		else
		{
			ErrInvalid(432, actual, socket_fd);
			return (false);
		}
	}
	return (true);
}

void Client::nickCommand(const std::string& command)
{
	std::istringstream cmdstr(command);
	std::vector<std::string> arglist;
	std::string segment;
	while (std::getline(cmdstr, segment, ' '))
	{
		while (!segment.empty() && (segment[segment.size() - 1] == '\n'
				|| segment[segment.size() - 1] == '\r'))
			segment.resize(segment.size() - 1);
		if (!segment.empty())
		{
			arglist.push_back(segment);
		}
	}
	if (arglist.size() < 2)
		ErrInvalid(431, this->getNick(), this->_socket_fd);
	else
	{
		std::string nickname = arglist[1];
		if (valid_charset(nickname, this->getNick(), this->_socket_fd))
		{
			if (isalpha(nickname[0]))
			{
				if (!this->_server->isNicknameAvailable(nickname))
					ErrInvalid(433, nickname, this->_socket_fd);
				if (nickname.size() > 9)
				{
					std::string newnick = RPL_NICK(this->getNick(), this->getUser(), nickname.substr(0, 9));
					this->_nickname = nickname.substr(0, 9);
					send(this->_socket_fd, newnick.c_str(), newnick.length(), 0);
				}
				else
				{
					std::string newnick = RPL_NICK(this->getNick(), this->getUser(), nickname);
					this->_nickname = nickname;
					send(this->_socket_fd, newnick.c_str(), newnick.length(), 0);
				}
				std::cout << "Nickname set to: " << this->_nickname << std::endl;
				if (!this->_authStep.isRegistered)
					this->_authStep.isNickSet = true;
			}
			else
				ErrInvalid(432, this->getNick(), this->_socket_fd);
		}
		else
			return ;
	}
}
