#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Irc.hpp"

class		Server;

typedef struct s_authenticationStep
{
	bool	isNickSet;
	bool	isUserSet;
	bool	isRegistered;
	bool	isPasswordSet;
}			t_authStep;

class Client
{
	private:
		Server      *_server; // pointer vers le l'intsance de Server (pour les channels)
		int         _socket_fd;
		std::string _username;
		std::string _hostname;
		std::string _servername;
		std::string _realname;
		std::string _nickname;
		t_authStep  _authStep;
		std::string _recvCommand;

		void		ircCommand(const std::string& command);
		void		nickCommand(const std::string& command);
		void		userCommand(const std::string& command);
		void		pingCommand(const std::string& command);
		void		quitCommand(const std::string& command);
		void		topicCommand(const std::string& command);
		void		partCommand(const std::string& command);
		void		joinCommand(const std::string& command);
		void		modeCommand(const std::string& command);
		void		privmsgCommand(const std::string& command);
		void		listCommand(const std::string& command);
		void		unavailableCommand(const std::string& command);
		void		authClient();

	public:
		Client();
		void        setServer(Server &server);
		void        setSocketFd(int fd);

		int         getSocketFd() const;
		void        handleCommand(std::string command);
  		std::string getNick() const;
};

#endif
