#include <exception>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>


struct bot_t{
	std::string chName;
	std::string chPwd;
	std::string svPwd;
	std::string svIP;
	int			svPort;
};

std::string intToString(int n){
	std::stringstream ss;
	ss << n;
	return ss.str();
}

int        charPToInt(const char *str)
{
    std::string s(str);
    
    std::stringstream ss(s);
    int n;
    ss >> n;
    return n;
}


int checkArgs(const char *port, const char *password){
	for (size_t i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
			return 1;
	}
	if (charPToInt(port) < 1024  || charPToInt(port) > 49151)
		return 2;
	if (strlen(password) < 6)
		return 3;
	return 0;
}

int	connectToIrc(bot_t bot){
	int sockFd;
	struct sockaddr_in servAddr;
	int opt = 1;

	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd == -1)
	{
		std::cerr << "Error: socket creation failed" << std::endl;
		exit(1);
	}
	if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) == -1)
	{
		std::cerr << "Error: setsockopt failed" << std::endl;
		exit(1);
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(bot.svIP.c_str());
	servAddr.sin_port = htons(bot.svPort);
	if (connect(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		std::cerr << "Error: connect failed" << std::endl;
		exit(1);
	}
	return sockFd;
}

void sendToIrc(int sockFd, std::string msg){
	send(sockFd, (msg + "\r\n").c_str(), msg.length() + 2, 0);
}

void joinToTarget(int sockFd, bot_t bot){
	srand(time(NULL));
	std::cout << "I'm Trying to join to " << std::endl;
	int randNum = rand() % 1000;
	std::string Botname = "BOT" + intToString(randNum);
	sleep(1);
	sendToIrc(sockFd, "PASS " + bot.svPwd);
	sleep(1);
	sendToIrc(sockFd, "NICK " + Botname);
	sleep(1);
	sendToIrc(sockFd, "USER " + Botname + " " + Botname + " " + Botname + " " + Botname);
	sleep(1);
	sendToIrc(sockFd, "JOIN " + bot.chName + " " + bot.chPwd);
	sleep(1);
}

void listen(bot_t bot)
{
	int sockFd = connectToIrc(bot);
	joinToTarget(sockFd, bot);
	while (1)
	{
		char buffer[256];
		ssize_t recvlen = recv(sockFd, buffer, 255, 0);
		if (recvlen == -1)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
			{
				continue;
			}
			else
			{
				std::cerr << "Error: Client Connection Error" << std::endl;
				close(sockFd);
				return;
			}
		}
		else if (recvlen == 0)
		{
			std::cerr << "Error: Unexpected Client Disconnection" << std::endl;
			close(sockFd);
			return;
		}
		buffer[recvlen] = '\0';
		if (strstr(buffer, "PING") != NULL)
		{
			std::string pong = "PONG " + std::string(buffer).substr(5);
			sendToIrc(sockFd, pong);
		}
		else if (strstr(buffer, "mkati"))
		{
			sendToIrc(sockFd, "PRIVMSG " + bot.chName + " :mkati is a good person");
		}
		else if (strstr(buffer, "exit"))
		{
			sendToIrc(sockFd, "PRIVMSG " + bot.chName + " :I'm leaving");
			close(sockFd);
			return;
		}
	}
}
	

int main(int ac, char **av){
	bot_t bot;

	if (ac == 5  || ac == 6)
	{
		switch (checkArgs(av[2], av[3]))
		{
		case 1:
			std::cerr << "Port must be a number" << std::endl;
			return(1);
		case 2:
			std::cerr << "Port must be between 1024 and 49151" << std::endl;
			return(1);
		case 3:
			std::cerr << "Password must be at least 6 characters long" << std::endl;
			return (1);
		default:
			bot.svIP = av[1];
			bot.svPort = charPToInt(av[2]);
			bot.svPwd = av[3];
			bot.chName = "#" + std::string(av[4]);
			bot.chPwd = (ac == 6) ? av[5] : "";
			break;
		}
		listen(bot);
	}
	else
	{
		std::cerr << "Usage: " << av[0] << "<server IP> <port> <password>  <channel name> <channel password> or without channel password" << std::endl;
		return 1;
	}
	return 0;
}