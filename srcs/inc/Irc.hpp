#ifndef IRC_HPP
# define IRC_HPP

# include <sstream>
# include <iostream>
# include <string>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <poll.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <map>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>

class Client;
class Channel;
class Server;

// Utils.cpp functions
std::vector<std::string>	split_cmd(std::string str, char sep);
std::string 				trim_cmd(const std::string& input);

#endif