#include "../INCS_BONUS/IrcServer.hpp"
#include "../INCS_BONUS/CommandExecuter.hpp"


IrcServer::IrcServer(const std::string &port, const std::string &password) : port(port), password(password)
{
    this->initializeServer();
    this->runServer();
}

IrcServer::~IrcServer()
{
    close(this->sockFd);
}

std::vector<t_client> &IrcServer::getClients()
{
    return this->clients;
}

std::vector<t_channel> &IrcServer::getChannels()
{
    return this->channels;
}

std::vector<struct pollfd> &IrcServer::getPollFds()
{
    return this->pollFds;
}

void IrcServer::initializeServer()
{
    struct sockaddr_in  servAddr;
    int                 opt = 1;

    this->sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockFd == -1)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        exit(1);
    }
    if (setsockopt(this->sockFd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        exit(1);
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(charPToInt(this->port.c_str()));
    if (bind(this->sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    {
        std::cerr << "Error: bind failed" << std::endl;
        exit(1);
    }
    if (listen(this->sockFd, 3) == -1)
    {
        std::cerr << "Error: listen failed" << std::endl;
        exit(1);
    }
    this->pollFds.resize(1);
    this->pollFds[0].fd = this->sockFd;
    this->pollFds[0].events = POLLIN;
}

void IrcServer::addClient(int fd, char *ip, int port)
{
    t_client client;

    client.fd = fd;
    client.isOp = false;
    client.isRegistered = false;
    client.passCorrect = false;
    client.nickName = "";
    client.userName = "";
    client.serverName = "";
    client.hostName = "";
    client.realName = "";
    client.invChannels.clear();
    client.client_ip = ip;
    client.client_port = port;
    this->clients.push_back(client);
}

void IrcServer::acceptConnection()
{
    struct sockaddr_in  clientAddr;
    socklen_t           clientAddrLen = sizeof(clientAddr);
    int                 newFd;

    newFd = accept(this->sockFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newFd == -1)
    {
        std::cerr << "Error: accept failed" << std::endl;
        return;
    }
    fcntl(newFd, F_SETFL, O_NONBLOCK);
    this->pollFds.resize(this->pollFds.size() + 1);
    this->pollFds[this->pollFds.size() - 1].fd = newFd;
    this->pollFds[this->pollFds.size() - 1].events = POLLIN;
    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":"  << clientAddr.sin_port << std::endl;
    send(newFd, ": 371 : Hello from server use command PASS\r\n", 45, 0);
    this->addClient(newFd, inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);
}

std::string IrcServer::messageParser(int fd)
{
    std::string ret = "";
    std::string recvMsg;
    char        buffer[256];
    bool        isComplated = false;

    while (!isComplated)
    {
        ssize_t recvlen = recv(fd, buffer, 255, 0);
        std::cout << buffer << std::endl;
        if (recvlen == -1)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                continue;
            }
            else
            {
                std::cerr << "Error: Client Connection Error" << std::endl;
                close(fd);
                return "";
            }
        }
        else if (recvlen == 0)
        {
            for (size_t i = 0; i < this->clients.size(); i++)
            {
                if (this->clients[i].fd == fd)
                {
                    std::cerr << "Error: Unexpected Client Disconnection That User Connects From " << this->clients[i].client_ip  << ":" << clients[i].client_port << std::endl;
                    for (size_t j = 0; j < this->channels.size(); j++)
                    {
                        for (size_t k = 0; k < this->channels[j].clients.size(); k++)
                        {
                            if (this->channels[j].clients[k].fd == fd)
                            {
                                CommandExecuter commandExecuter(this, "PART " + this->channels[j].name, &this->channels[j].clients[k]);
                                this->channels[j].clients.erase(this->channels[j].clients.begin() + k);
                            }
                        }
                    }
                    this->pollFds.erase(this->pollFds.begin() + i + 1);
                    this->clients.erase(this->clients.begin() + i);
                    close(fd);
                    return "";
                }
            }
        }
        buffer[recvlen] = '\0';
        recvMsg += buffer;
        if (recvMsg.at(recvMsg.length() - 1) == '\n')
            isComplated = true;
    }
    size_t pos = recvMsg.find_first_of("\r\n");
    if (pos != std::string::npos)
    {
        ret = recvMsg.substr(0, pos);
    }
    else
    {
        ret = recvMsg;
    }
    return ret;
}

void IrcServer::handleMessage()
{
    for (size_t i = 1; i < this->pollFds.size(); i++) 
    {
        if (this->pollFds[i].revents & POLLHUP) 
        {
            close(this->pollFds[i].fd);
            this->pollFds.erase(this->pollFds.begin() + i);
            this->clients.erase(this->clients.begin() + i - 1);
            std::cout << "Client disconnected" << std::endl;
            continue;
        }
        else if (this->pollFds[i].revents & POLLIN) 
        {
            std::string msg = this->messageParser(this->pollFds[i].fd);
            if (!msg.empty()) 
            {
                CommandExecuter commandExecuter(this, msg, &this->clients[i -1]);
            }
        }
    }
    if (this->pollFds[0].revents & POLLIN)
    {
        this->acceptConnection();
    }
}

void IrcServer::runServer()
{
    while (poll(&this->pollFds[0], this->pollFds.size(), -1))
        this->handleMessage();
}
