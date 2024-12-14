#ifndef MAIN_HPP
# define MAIN_HPP

# include <map>
# include <stdio.h>
# include <unistd.h>
# include <iostream>
# include <arpa/inet.h>
# include <string.h>
# include <vector>
# include <fcntl.h>
# include <sstream>
# include <poll.h>
# include <climits>
# include <sys/socket.h>
# include <stdlib.h>
# include <algorithm>
# include <netinet/in.h>
# include <cerrno>

int         checkArgs(const char *port, const char *password);
std::string size_tToString(size_t n);

#endif