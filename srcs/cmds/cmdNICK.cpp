// #include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <sstream>
#include <string>


static void	ErrInvalid(int error_n, std::string err_arg)
{
	if (error_n == 431)
        std::cout << ":localhost 431 " << err_arg << ":No nickname given" << std::endl;
	else if (error_n == 432)
		std::cout << ":localhost 432 " << err_arg << ":Erroneus nickname" << std::endl;
	else if (error_n == 433)
		std::cout << ":localhost :Nickname is already in use" << std::endl;
	else
		return ;
}

static bool	valid_charset(std::string nick, std::string actual)
{
	int	i;

	i = 0;
	while (nick[i])
	{
		if (isalnum(nick[i]) || nick[i] == '{' || nick[i] == '}'
			|| nick[i] == '[' || nick[i] == ']' || nick[i] == '\\'
			|| nick[i] == '|')
            i++;
        else
		{
			ErrInvalid(432, actual);
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
	std::string nickname;

	while (std::getline(cmdstr, segment, ' '))
	{
		if (!segment.empty())
			arglist.push_back(segment);
	}

    if (arglist.size() < 2) {
		ErrInvalid(431, this->getNick());
	}
	else {
		nickname = command.substr(command.find(' ') + 1, 35);

		if (nickname.size() < 9 && valid_charset(nickname, this->getNick()))
		{
			if (isalpha(nickname[0]))
			{
				//if (nameDuplicated(nickname))
				//	ErrInvalid(433, nickname);
				this->_nickname = nickname;
				std::cout << "Nickname set to: " << this->_nickname << std::endl;
				if (!this->_authStep.isRegistered)
				this->_authStep.isNickSet = true;
			}
			else
				ErrInvalid(432, this->getNick());
		}
		else
			return ;
	}
}
