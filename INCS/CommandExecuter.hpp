#ifndef COMMAND_EXECUTER_HPP
# define COMMAND_EXECUTER_HPP

#include "Client.hpp"
#include "IrcServer.hpp"

class CommandExecuter
{
    private:
        IrcServer                   *server;
        std::string                 message;
        std::vector<std::string>    splitMessage;
        t_client                    *client;
    public:
        CommandExecuter(IrcServer *server, std::string message, t_client *client);
        ~CommandExecuter();
        void            sendMessage(std::string message, int fd);
        std::string     formatMessage(t_client client);

        void    executePassCommand();
        void    executeNickCommand();
        void    executeUserCommand();
        void    executeJoinCommand();
        void    executePartCommand();
        void    executePrivmsgCommand();
        void    executeWhoCommand();
        void    executeNoticeCommand();
        void    executeModeCommand();
        void    executePingCommand();
        void    executeTopicCommand();
        void    executeListCommand();
        void    executeQuitCommand();
        void    executeKickCommand();
        void    executeInviteCommand();
};

#endif