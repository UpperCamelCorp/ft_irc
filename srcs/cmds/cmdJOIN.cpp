#include "Server.hpp"
#include "Irc.hpp"

static void	ErrInvalid(int error_n, std::string err_arg, int socket_fd)
{
	std::string response;
	if (error_n == 461)
		response = ":localhost 461 " + err_arg + " :No enough parameters\n";
	else if (error_n == 403)
		response = ":localhost " + err_arg + " :No such channel\n"; // channel
	else if (error_n == 405)
		response = ":localhost " + err_arg + " :You have joined too many channels\n"; // channel
	else if (error_n == 475)
		response = ":localhost " + err_arg + " :Cannot join channel (+k)\n"; // bad key
	else if (error_n == 474)
		response = ":localhost " + err_arg + " :Cannot join channel (+b)\n";
	else if (error_n == 471)
		response = ":localhost " + err_arg + " :Cannot join channel (+l)\n";
	else if (error_n == 473)
		response = ":localhost " + err_arg + " :Cannot join channel (+i)\n";
	else if (error_n == 476)
		response = ":localhost " + err_arg + " :Bad Channel Mask\n"; // bad name format
	else
		return ;
	send(socket_fd, response.c_str(), response.length(), 0);
}

void Client::joinCommand(const std::string& command)
{
	std::vector<std::string> arglist;
	arglist = split_cmd(command, ' ');
	if (arglist.size() < 2)
		return (ErrInvalid(461, this->getNick(), this->_socket_fd));

	std::vector<std::string> channels;
	channels = split_cmd(arglist[1], ',');

	std::vector<std::string> passwords;
	if (arglist.size() == 3)
		passwords = split_cmd(arglist[2], ',');
	std::map<std::string, Channel>& serverChannels = this->_server->getChannels();
	std::map<std::string, Channel>::iterator it;

	size_t pass_i = 0;
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (!valid_channel_name(channels[i]))
		{
			ErrInvalid(476, channels[i], this->_socket_fd);
			continue;
		}
		else if (serverChannels.find(channels[i]) != serverChannels.end())
		{
			it = serverChannels.find(channels[i]);
			if (it->second.isInviteOnly() && !it->second.isClientInvited(*this))
			{
				ErrInvalid(473, it->second.getName(), this->_socket_fd);
				continue;
			}
			if (it->second.getPassword() != "")
			{
				std::string provided_password = (pass_i < passwords.size()) ? passwords[pass_i] : "";
				if (!it->second.addClient(*this, provided_password))
				{
					ErrInvalid(475, it->second.getName(), this->_socket_fd);
					std::string partCmd = "PART " + it->second.getName() + "\r\n";
					this->partCommand(partCmd);
					pass_i++;
					continue;
				}
				pass_i++;
			}
			else
				it->second.addClient(*this, "");
		}
		else
		{
			serverChannels.insert(std::make_pair(channels[i], Channel(channels[i])));
			it = serverChannels.find(channels[i]);
			it->second.addOperator(*this);
			it->second.addClient(*this, "");
			std::cout << "created and joined " << channels[i] << std::endl;
		}

        std::string join_msg = ":" + this->getNick() + " JOIN :" + channels[i] + "\r\n";
        send(this->_socket_fd, join_msg.c_str(), join_msg.length(), 0);

        if (!it->second.getTopic().empty())
        {
            std::string topic_msg = ":localhost 332 " + this->getNick() + " " + channels[i] + " :" + it->second.getTopic() + "\r\n";
            send(this->_socket_fd, topic_msg.c_str(), topic_msg.length(), 0);
        }

        std::vector<Client> channel_clients = it->second.getClients();
        std::string names = "";
        for (std::vector<Client>::iterator client_it = channel_clients.begin(); client_it != channel_clients.end(); ++client_it)
        {
            if (it->second.isOperator(*client_it))
                names += "@";
            names += client_it->getNick() + " ";
        }
        std::string names_msg = ":localhost 353 " + this->getNick() + " = " + channels[i] + " :" + names + "\r\n";
        send(this->_socket_fd, names_msg.c_str(), names_msg.length(), 0);

        std::string end_names_msg = ":localhost 366 " + this->getNick() + " " + channels[i] + " :End of /NAMES list\r\n";
        send(this->_socket_fd, end_names_msg.c_str(), end_names_msg.length(), 0);

        it->second.sendMessage(join_msg, *this);

        std::cout << this->getNick() << " joined " << channels[i] << std::endl;
	}
}
