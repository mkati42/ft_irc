#include <exception>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <signal.h>

struct bot_t{
	std::string chName;
	std::string chPwd;
	std::string svPwd;
	std::string svIP;
	int			svPort;
	int			sockFd;
};

static bot_t bot;

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


std::string extractBotCommand(const std::string& fullMsg) {
    size_t msgStart = fullMsg.find(" PRIVMSG ");
    if (msgStart == std::string::npos) return "";

    size_t contentStart = fullMsg.find(" :", msgStart);
    if (contentStart == std::string::npos) return "";

    return fullMsg.substr(contentStart + 2);
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
		return -1;
	}
	if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)) == -1)
	{
		std::cerr << "Error: setsockopt failed" << std::endl;
		return -1;
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(bot.svIP.c_str());
	servAddr.sin_port = htons(bot.svPort);
	if (connect(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		std::cerr << "Error: connect failed" << std::endl;
		close(sockFd);
		return -1;
	}
	return sockFd;
}

void sendToIrc(int sockFd, std::string msg){
	send(sockFd, (msg + "\r\n").c_str(), msg.length() + 2, 0);
}

void joinToTarget(int sockFd, bot_t bot){
	srand(time(NULL));
	std::cout << "I'm Trying to join to " << bot.svIP << ":" << bot.svPort << "" << bot.chName << std::endl;
	int randNum = rand() % 100000;
	std::string Botname = "BOT" + intToString(randNum);
	sendToIrc(sockFd, "PASS " + bot.svPwd);
	sleep(1);
	sendToIrc(sockFd, "NICK " + Botname);
	sleep(1);
	sendToIrc(sockFd, "USER " + Botname + " " + Botname + " " + Botname + " " + Botname);
	sleep(1);
	sendToIrc(sockFd, "JOIN " + bot.chName + " " + bot.chPwd);
}

void signalHandler(int signum)
{
	(void)signum;
	sendToIrc(bot.sockFd, "QUIT :Bot is leaving");
	sleep(1);
	close(bot.sockFd);
	bot.sockFd = -1;
}

void listen(bot_t *bot)
{
	int sockFd = connectToIrc(*bot);
	if (sockFd == -1)
		return;
	bot->sockFd = sockFd;
	joinToTarget(sockFd, *bot);
	signal(SIGINT, signalHandler);
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
				if (bot->sockFd == -1)
				{
					std::cout << "Bot is closed with signal" << std::endl;
					return;
				}
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
		std::string msg(buffer);
		if (extractBotCommand(msg) == "bot.mkati\r\n")
		{
			sendToIrc(sockFd, "PRIVMSG " + bot->chName + " :Mkati is a good guy");
		}
		else if (extractBotCommand(msg) == "bot.hello\r\n")
		{
			sendToIrc(sockFd, "PRIVMSG " + bot->chName + " :Hello, I'm a bot");
		}
		else if (extractBotCommand(msg) == "bot.iloveyou\r\n")
		{
			sendToIrc(sockFd, "PRIVMSG " + bot->chName + " :I love you too <3");
		}
		else if (extractBotCommand(msg) == "bot.quit\r\n")
		{
			sendToIrc(sockFd, "QUIT :Bot is leaving");
			close(sockFd);
			return;
		}
	}
}
	

int main(int ac, char **av){

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
		listen(&bot);
	}
	else
	{
		std::cerr << "Usage: " << av[0] << "<server IP> <port> <password>  <channel name> <channel password> or without channel password" << std::endl;
		return 1;
	}
	return 0;
}