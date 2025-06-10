// #include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <sstream>
#include <string>

// static bool	valid_nick(std::string nick) {
// 	if (isdigit(nick[0]))
// 		return (false);
// 	if (isalpha(nick[0]))
// 		return (true);
// }

// std::stringstream test("this_is_a_test____string");
// std::string segment;
// std::vector<std::string> seglist;

// while(std::getline(test, segment, '_')) {
//     if (!segment.empty())
//         seglist.push_back(segment);
// }

// for (size_t i = 0; i < seglist.size(); ++i) {
//     std::cout << seglist[i] << std::endl;
// }

// return (0);



static bool	valid_charset(std::string nick)
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
            return false;
	}
    return true;
}

void Client::nickCommand(std::string command)
{
	std::istringstream cmdstr(command);

	std::vector<std::string> seglist;
	std::string segment;
	std::string nickname;

	while (std::getline(cmdstr, segment, ' '))
	{
		if (!segment.empty())
			seglist.push_back(segment);
	}

    if (seglist.size() < 2)
        std::cout << "<client> :No nickname given" << std::endl;
    std::cout << " TESTTT " << seglist[0] << std::endl;

	if (seglist[1].size() < 6 && (isalpha(seglist[1][0]) && valid_charset(seglist[1])))
		std::cout << "OK : " << command << std::endl;
	else
		std::cout << "command is : " << command << std::endl; // error case throw

	{
		std::cout << "Found : " << command.substr(command.find(' ')) << std::endl;
	}
	nickname = command.substr(command.find(' ') + 1, 35);
	if (nickname.empty())
	{
		std::cout << "Error: NICK command requires a nickname." << std::endl;
		return ;
	}
	this->_nickname = nickname;
	std::cout << "Nickname set to: " << this->_nickname << std::endl;
	if (!this->_authStep.isRegistered)
		this->_authStep.isNickSet = true;
}

/* USE POLL FOR ERR MSG
   OR WRITE IT BUT*/