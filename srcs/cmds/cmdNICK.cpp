// #include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include <sstream>
#include <string>

// static bool	valid_nick(std::string nick) {
// 	if (isdigit(nick[0]))
// 		return false;
// 	if (isalpha(nick[0]))
// 		return true;
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


    // return 0;

void	Client::nickCommand(std::string command)
{
    std::string segment;
    std::vector<std::string> seglist;
    std::string nickname;

	while(std::getline(command, segment, '_')) {
        if (!segment.empty())
            seglist.push_back(segment);

	if (command.size() < 6 || isalpha(command[5]))
		std::cout << "OK : " << command << std::endl;
	else
		std::cout << "command is : " << command << std::endl;

	{

		std::cout << "Found : " << command.substr(command.find(' ') );
	}
    nickname = command.substr(command.find(' ') + 1, 35);
    if (nickname.empty())
    {
        std::cout << "Error: NICK command requires a nickname." << std::endl;
        return;
    }
    this->_nickname = nickname;
    std::cout << "Nickname set to: " << this->_nickname << std::endl;
    if (!this->_authStep.isRegistered)
        this->_authStep.isNickSet = true;

}