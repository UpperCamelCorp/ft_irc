#include "../inc/Client.hpp"

static void	ErrInvalid(int error_n, std::string err_arg, int socket_fd)
{
	std::string	response;

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
		std::cout << nick[i] << std::endl;
		if (isalnum(nick[i]) || nick[i] == '{' || nick[i] == '}'
			|| nick[i] == '[' || nick[i] == ']' || nick[i] == '\\'
			|| nick[i] == '|' || (i > 0 && nick[i] == '\n'))
				i++;
        else
		{
			ErrInvalid(432, actual, socket_fd);
            return false;
		}
	}
    return true;
}

void Client::nickCommand(std::string command)
{
	std::istringstream cmdstr(command);

	std::vector<std::string> arglist;
	std::string segment;

	while (std::getline(cmdstr, segment, ' '))
	{
		if (!segment.empty() && segment[segment.length() - 1] == '\n')
		{
		    segment = segment.substr(0, segment.length() - 1);
		    if (!segment.empty() && segment[segment.length() - 1] == '\r')
			{
		    	segment = segment.substr(0, segment.length() - 1);
		    }
			arglist.push_back(segment);
 		}
		else if (!segment.empty())
		{
			arglist.push_back(segment);
		}
	}
			
    if (arglist.size() < 2) {
		ErrInvalid(431, this->getNick(), this->_socket_fd);
	}
	else {
		std::string nickname = arglist[1];

		if (valid_charset(nickname, this->getNick(), this->_socket_fd))
		{
			if (isalpha(nickname[0]))
			{
				//if (nameDuplicated(nickname))
				//	ErrInvalid(433, nickname);
				if (nickname.size() > 9)
					this->_nickname = nickname.substr(0, 9);
				else
					this->_nickname = nickname;
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
