#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "main.hpp"

typedef struct s_client
{
    std::string                 nickName;
    std::string                 userName;
    std::string                 serverName;
    std::string                 hostName;
    std::string                 realName;
    int                         fd;
    bool                        isOp;
    bool                        isRegistered;
    bool                        passCorrect;
    std::vector<std::string>    invChannels;

    char*                       client_ip;
    int                         client_port;
}                               t_client;






#endif