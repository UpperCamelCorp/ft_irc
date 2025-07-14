#include "Irc.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ErrMacro.hpp"

static bool	valid_charset(std::string nick)
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
			return (false);
	}
	return (true);
}

void Client::nickCommand(const std::string& command)
{
	std::istringstream cmdstr(command);
	std::vector<std::string> arglist;
	std::string segment;
	std::string response;
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
	{
		response = ERR_NONICKNAMEGIVEN(this->getNick());
		send(_socket_fd, response.c_str(), response.length(), 0);
	}
	else
	{
		std::string nickname = arglist[1];
		if (valid_charset(nickname) && isalpha(nickname[0]))
		{
			if (!this->_server->isNicknameAvailable(nickname))
			{
				response = ERR_NICKNAMEINUSE(this->getNick(), nickname);
				send(_socket_fd, response.c_str(), response.length(), 0);
				return ;
			}
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
		{
			response = ERR_ERRONEUSNICKNAME(this->getNick(), nickname);
			send(_socket_fd, response.c_str(), response.length(), 0);
			return ;
		}
	}
}
