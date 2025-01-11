#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include "main.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class IrcServer
{
    private:
        std::string                 port;
        std::string                 password;
        int                         sockFd;
        std::vector<struct pollfd>  pollFds;
        std::vector<t_client>       clients;
        std::vector<t_channel>      channels;

    public:
        IrcServer(const std::string &port, const std::string &password);
        ~IrcServer();

        void                        runServer();
        void                        initializeServer();
        void                        handleMessage();
        void                        acceptConnection();
        void                        addClient(int fd, char *ip, int port);
        std::string                 messageParser(int fd);
        std::vector<t_client>       &getClients();
        std::string                 getPassword() const { return this->password; }
        std::vector<t_channel>      &getChannels();
        std::vector<struct pollfd>  &getPollFds();
};


#endif