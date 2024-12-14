#include "../INCS/CommandExecuter.hpp"

CommandExecuter::CommandExecuter(IrcServer *server, std::string message, t_client *client) : server(server), message(message), client(client)
{
    std::stringstream ss(message);
    std::string token;

    while (std::getline(ss, token, ' '))
    {
        this->splitMessage.push_back(token);
    }

    if (this->splitMessage[0] == "PASS")
    {
        this->executePassCommand();
    }
    else if (this->splitMessage[0] == "NICK")
    {
        this->executeNickCommand();
    }
    else if (this->splitMessage[0] == "USER")
    {
        this->executeUserCommand();
    }
    else if (this->splitMessage[0] == "JOIN")
    {
        this->executeJoinCommand();
    }
    else if (this->splitMessage[0] == "PRIVMSG")
    {
        this->executePrivmsgCommand();
    }
	else if (this->splitMessage[0] == "PART")
	{
		this->executePartCommand();
	}
	else if (this->splitMessage[0] == "PING")
	{
		this->executePingCommand();
	}
	else if (this->splitMessage[0] == "NOTICE")
	{
		this->executeNoticeCommand();
	}
	else if (this->splitMessage[0] == "MODE")
	{
		this->executeModeCommand();
	}
	else if(this->splitMessage[0] == "TOPIC")
	{
		this->executeTopicCommand();
	}
	else if(this->splitMessage[0] == "KICK")
	{
		this->executeKickCommand();
	}
	else if (this->splitMessage[0] == "LIST")
	{
		this->executeListCommand();
	}
	else if (this->splitMessage[0] == "WHO")
	{
		this->executeWhoCommand();
	}
	else if (this->splitMessage[0] == "INVITE")
	{
		this->executeInviteCommand();
	}
	else if (this->splitMessage[0] == "QUIT")
	{
		this->executeQuitCommand();
	}
	else
	{
		this->sendMessage(":" + client->hostName + " 421 " + client->nickName + " " + this->splitMessage[0] + " :Unknown command\r\n", client->fd);
	}
}

CommandExecuter::~CommandExecuter()
{
}

void CommandExecuter::sendMessage(std::string message, int fd)
{
    send(fd, message.c_str(), message.length(), 0);
}

std::string CommandExecuter::formatMessage(t_client client)
{
    return ":" + client.nickName + "!" + client.userName + "@" + client.hostName;
}

void CommandExecuter::executePassCommand()
{
    if (this->splitMessage.size() != 2)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " PASS : Usage is PASS <PASSWORD>. You use too much parameters or your command\r\n", client->fd);
        return;
    }
    if (this->client->passCorrect)
    {
        this->sendMessage(":" + client->hostName + " 462 " + client->nickName + " PASS :You are already authorized\r\n", client->fd);
        return;
    }
    if (this->splitMessage[1] != this->server->getPassword())
    {
        this->sendMessage(": 464  PASS :Password incorrect\r\n", client->fd);
        return;
    }
    this->client->passCorrect = true;
    this->sendMessage(": 371 :Use the NICK command to set your nickname\r\n", client->fd);


}

void CommandExecuter::executeNickCommand()
{
    if (this->splitMessage.size() != 2)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " NICK : Usage is NICK <NICKNAME>. You use too much parameters or your command\r\n", client->fd);
        return;
    }
    if (!this->client->passCorrect)
    {
        this->sendMessage(":" + client->hostName + " 451 " + client->nickName + " NICK :You have not registered\r\n", client->fd);
        return;
    }
    for (size_t i = 0; i < this->server->getClients().size(); i++)
    {
        if (this->server->getClients()[i].nickName == this->splitMessage[1] && this->server->getClients()[i].fd != this->client->fd)
        {
            this->sendMessage(":" + client->hostName + " 433 " + client->nickName + " NICK :Nickname is already in use\r\n", client->fd);
            return;
        }
    }
    this->client->nickName = this->splitMessage[1];
    this->sendMessage(": 371 :Use the USER command to set your user\r\n", client->fd);
}

void CommandExecuter::executeUserCommand()
{
    if (this->splitMessage.size() != 5)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " USER : Usage is USER <USERNAME> <HOSTNAME> <SERVERNAME> <REALNAME>. You use too much parameters or your command\r\n", client->fd);
        return;
    }
    if (client->isRegistered)
    {
        this->sendMessage(":" + client->hostName + " 462 " + client->nickName + " USER :You are already registered\r\n", client->fd);
        return;
    }
    if (!this->client->passCorrect)
    {
        this->sendMessage(":" + client->hostName + " 451 " + client->nickName + " USER :You have not registered\r\n", client->fd);
        return;
    }
    if (!this->client->nickName.empty())
    {
        this->client->userName = this->splitMessage[1];
        this->client->serverName = this->splitMessage[2];
        this->client->hostName = this->splitMessage[3];
        this->client->realName = this->splitMessage[4];
    }
    for (size_t i = 0; i < this->server->getClients().size(); i++)
    {
        if ((this->server->getClients()[i].userName == this->client->userName ||
            this->server->getClients()[i].hostName == this->client->hostName ||
            this->server->getClients()[i].serverName == this->client->serverName ||
            this->server->getClients()[i].realName == this->client->realName) && this->server->getClients()[i].fd != this->client->fd)
        {
            this->sendMessage(":" + client->hostName + " 462 " + client->nickName + " USER :You are already registered\r\n", client->fd);
            return;
        }
    }
    this->client->isRegistered = true;
    this->sendMessage(this->formatMessage(*client) + "You are now registered\r\n", client->fd);
    this->sendMessage(": 001 " + client->nickName + " :Welcome to the Internet Relay Network\r\n", client->fd);
    std::cout << "Client registered  " + client->nickName << " fd " << client->fd << std::endl;
}

void CommandExecuter::executeJoinCommand()
{
    if (this->splitMessage.size() < 2)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " JOIN : Usage is JOIN <#CHANNEL>. You use too much parameters or your command\r\n", client->fd);
        return;
    }
    if (!this->client->isRegistered)
    {
        this->sendMessage(":" + client->hostName + " 451 " + client->nickName + " JOIN :You have not registered\r\n", client->fd);
        return;
    }
    if (this->splitMessage[1][0] != '#')
    {
        this->sendMessage(":" + client->hostName + " 403 " + client->nickName + " JOIN :No such channel\r\n", client->fd);
        return;
    }
    for (size_t i = 0; i < this->server->getChannels().size(); i++)
    {
        if (this->server->getChannels()[i].name == this->splitMessage[1])
        {
            for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
            {
                if (this->server->getChannels()[i].clients[j].fd == this->client->fd)
                {
                    this->sendMessage(":" + client->hostName + " 405 " + client->nickName + " JOIN :You are already in that channel\r\n", client->fd);
                    return;
                }
            }
            if (this->server->getChannels()[i].isSecret && std::find(this->client->invChannels.begin(), this->client->invChannels.end(), this->splitMessage[1]) == this->client->invChannels.end())
            {
                this->sendMessage(":" + client->hostName + " 473 " + client->nickName + " JOIN :Channel is invite only\r\n", client->fd);
                return;
            }
            if (this->server->getChannels()[i].limit <= this->server->getChannels()[i].clients.size())
            {
                this->sendMessage(":" + client->hostName + " 471 " + client->nickName + " JOIN :Channel is full\r\n", client->fd);
                return;
            }
            if (this->server->getChannels()[i].key != "" && this->splitMessage[2] != this->server->getChannels()[i].key)
            {
                this->sendMessage(":" + client->hostName + " 475 " + client->nickName + " JOIN :Bad channel key\r\n", client->fd);
                return;
            }
            this->server->getChannels()[i].clients.push_back(*client);
            if (std::find(this->client->invChannels.begin(), this->client->invChannels.end(), this->splitMessage[1]) != this->client->invChannels.end())
            {
                this->client->invChannels.erase(std::remove(this->client->invChannels.begin(), this->client->invChannels.end(), this->splitMessage[1]), this->client->invChannels.end());
            }
            this->sendMessage(this->formatMessage(*client) + " JOIN " + this->splitMessage[1] + "\r\n", client->fd);
            for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
            {
                for(size_t k = 0; k < this->server->getChannels()[i].clients.size(); k++)
                {
                    if(this->server->getChannels()[i].clients[j].fd != this->server->getChannels()[i].clients[k].fd)
                    {
                        this->sendMessage(this->formatMessage(this->server->getChannels()[i].clients[k]) + " JOIN " + this->splitMessage[1] + "\r\n", this->server->getChannels()[i].clients[j].fd);
                    }
					if (this->server->getChannels()[i].clients[k].isOp)
						this->sendMessage("MODE " + this->splitMessage[1] + " +o " + this->server->getChannels()[i].clients[k].nickName + "\r\n", this->server->getChannels()[i].clients[j].fd);
                }
            }
            return;
        }
    }
	if (this->splitMessage[1][0] == '#' && this->splitMessage[1].size() > 1)
    {
		t_channel newChannel;
    	newChannel.name = this->splitMessage[1];
    	newChannel.clients.push_back(*client);
		newChannel.operatorList.push_back(client->fd);
		newChannel.clients[0].isOp = true;
    	newChannel.topic = "";
    	newChannel.key = "";
    	newChannel.limit = INT_MAX;
    	newChannel.isSecret = false;
		newChannel.secretTopic = false;
    	this->server->getChannels().push_back(newChannel);
    	this->sendMessage(this->formatMessage(*client) + " JOIN " + this->splitMessage[1] + "\r\n", client->fd);
		this->sendMessage("MODE " + newChannel.name + " +o " + client->nickName + "\r\n", client->fd);
	}
	else
	{
		this->sendMessage(this->formatMessage(*client) + "need #\r\n", client->fd);
		return;
	}
}

void CommandExecuter::executePartCommand()
{
	if (this->splitMessage.size() < 1)
	{
		this->sendMessage(": 461 PART :Not enough parameters\r\n", client->fd);
		return;
	}
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[1] == this->server->getChannels()[i].name)
		{
			for(size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
			{
				if (client->fd == this->server->getChannels()[i].clients[j].fd)
				{
					for (size_t k = 0; k < this->server->getChannels()[i].clients.size(); k++)
						this->sendMessage(this->formatMessage(*client) + " PART " + this->splitMessage[1] + "\r\n", this->server->getChannels()[i].clients[k].fd);
					this->server->getChannels()[i].clients.erase(this->server->getChannels()[i].clients.begin() + j);
					if (this->server->getChannels()[i].clients.empty())
						this->server->getChannels().erase(this->server->getChannels().begin() + i);
					else if (this->server->getChannels()[i].operatorList.size() == 1 && this->server->getChannels()[i].operatorList[0] == client->fd)
					{
						this->server->getChannels()[i].operatorList.erase(this->server->getChannels()[i].operatorList.begin() + 0);
						this->server->getChannels()[i].clients[0].isOp = true;
						this->server->getChannels()[i].operatorList.push_back(this->server->getChannels()[i].clients[0].fd);
						for (size_t l = 0; l < this->server->getChannels()[i].clients.size(); l++)
							this->sendMessage("MODE " + this->splitMessage[1] + " +o " + this->server->getChannels()[i].clients[0].nickName + "\r\n", this->server->getChannels()[i].clients[l].fd);
					}
					else
					{
						for (size_t t = 0; t < this->server->getChannels()[i].operatorList.size(); t++)
							if (client->fd == this->server->getChannels()[i].operatorList[t])
								this->server->getChannels()[i].operatorList.erase(this->server->getChannels()[i].operatorList.begin() + t);
					}

					return;
				}
			}
		}
	}
}


void CommandExecuter::executePrivmsgCommand()
{
    if (this->splitMessage.size() < 3)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " PRIVMSG : Usage is PRIVMSG <NICKNAME> <MESSAGE>. You use too much parameters or your command\r\n", client->fd);
        return;
    }
    if (!this->client->isRegistered)
    {
        this->sendMessage(":" + client->hostName + " 451 " + client->nickName + " PRIVMSG :You have not registered\r\n", client->fd);
        return;
    }
    std::string message = "";
	for (size_t i = 2; i != this->splitMessage.size(); i++)
	{
		message += this->splitMessage[i];
		if (i != this->splitMessage.size() - 1)
			message += " ";
	}
    for (size_t i = 0; i < this->server->getClients().size(); i++)
    {
        if (this->server->getClients()[i].nickName == this->splitMessage[1])
        {
            this->sendMessage(this->formatMessage(*client) + " PRIVMSG " + this->splitMessage[1] + " :" + message + "\r\n", this->server->getClients()[i].fd);
            return;
        }
    }
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[1] == this->server->getChannels()[i].name)
		{
			int flag = 0;
			for (size_t l = 0; l < this->server->getChannels()[i].clients.size(); l++)
			{
				if (this->server->getChannels()[i].clients[l].fd == client->fd)
				{
					flag = 1;
					break;
				}
			}
			if (!flag)
			{
				this->sendMessage(":" + client->hostName + " 442 " + client->nickName + " PRIVMSG :You're not on that channel\r\n", client->fd);
				return;
			}
			for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
			{
				if (this->server->getChannels()[i].clients[j].fd != client->fd)
					this->sendMessage(this->formatMessage(*client) + " PRIVMSG " + this->splitMessage[1] + " :" + message + "\r\n", this->server->getChannels()[i].clients[j].fd);
			}
			return;
		}
	}
    this->sendMessage(":" + client->hostName + " 401 " + client->nickName + " PRIVMSG :No such nick/channel\r\n", client->fd);
}

void CommandExecuter::executeWhoCommand()
{
	if (this->splitMessage.size() < 2)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " WHO : Usage is WHO <#CHANNEL>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	else if (this->splitMessage.size() > 2)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " WHO : Usage is WHO <#CHANNEL>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	else
	{
		for (size_t i = 0; i < this->server->getChannels().size(); i++)
		{
			if (this->server->getChannels()[i].name == this->splitMessage[1])
			{
				for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
					sendMessage(":AUMSERV 352 " + this->server->getChannels()[i].name +
					" " + this->server->getChannels()[i].name.substr(1, this->server->getChannels()[i].name.length()) + " " +
					this->server->getChannels()[i].clients[j].userName + " " +
					this->server->getChannels()[i].clients[j].hostName + " AUMSERV " +
					this->server->getChannels()[i].clients[j].nickName + " 1 " +
					this->server->getChannels()[i].clients[j].realName + " H :0 " +
					this->server->getChannels()[i].clients[j].realName + "\r\n", client->fd);
			}
		}
	}
}

void CommandExecuter::executePingCommand()
{
	if (client->isRegistered)
	{
		if (this->splitMessage.size() == 1)
			this->sendMessage("PONG\r\n", client->fd);
		else if(this->splitMessage.size() > 1)
			this->sendMessage("PONG " + this->splitMessage[1] + "\r\n", client->fd);
	}
}

void CommandExecuter::executeNoticeCommand()
{
	if (this->splitMessage.size() < 3)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " NOTICE : Usage is NOTICE <#CHANNEL> <MESSAGE>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	std::string message = "";
	for (size_t i = 2; i < this->splitMessage.size(); i++)
	{
		message += this->splitMessage[i];
		if (i != this->splitMessage.size() - 1)
			message += " ";
	}
	if (this->splitMessage[1][0] == '#')
	{
		for (size_t i = 0; i < this->server->getChannels().size(); i++)
		{
			if (this->server->getChannels()[i].name == this->splitMessage[1])
			for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
				this->sendMessage(this->formatMessage(*client) + " NOTICE " + this->splitMessage[1] + " :" + message + "\r\n", this->server->getChannels()[i].clients[j].fd);
			return;
		}
		this->sendMessage(this->formatMessage(*client) + " :Unknown Channel Name or Client Name " + "\r\n", client->fd);
	}
	else
	{
		for (size_t i = 0; i < this->server->getClients().size(); i++)
		{
			if (this->server->getClients()[i].nickName == this->splitMessage[1])
			{
				this->sendMessage(this->formatMessage(*client) + " NOTICE " + this->splitMessage[1] + " :" + message + "\r\n", this->server->getClients()[i].fd);
				return;
			}
		}
		this->sendMessage(this->formatMessage(*client) + " :Unknown Channel Name or Client Name " + "\r\n", client->fd);
	}
}

void CommandExecuter::executeModeCommand()
{

	if (this->splitMessage.size() < 3)
	{
		return;
	}
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[1] == this->server->getChannels()[i].name)
		{
			for(size_t j = 0; j < this->server->getChannels()[i].operatorList.size(); j++)
			{
				if (this->server->getChannels()[i].operatorList[j] == client->fd)
				{
					if (this->splitMessage[2] == "+i" || this->splitMessage[2] == "-i")
					{
						this->server->getChannels()[i].isSecret = (this->splitMessage[2] == "+i") ? true : false;
						for (size_t t = 0; t < this->server->getChannels()[i].clients.size(); t++)
							this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " " + this->splitMessage[2] + "\r\n", this->server->getChannels()[i].clients[t].fd);
						return;
					}
					else if (this->splitMessage[2] == "+k" || this->splitMessage[2] == "-k")
					{
						if (this->splitMessage[2] == "+k")
							this->server->getChannels()[i].key = this->splitMessage[3];
						else
							this->server->getChannels()[i].key = "";
						for (size_t t = 0; t < this->server->getChannels()[i].clients.size(); t++)
							this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " " + this->splitMessage[2] + " " + ((this->splitMessage[2] == "+k") ? this->splitMessage[3] : "") + "\r\n", this->server->getChannels()[i].clients[t].fd);
						return;
					}
					else if (this->splitMessage[2] == "+t" || this->splitMessage[2] == "-t")
					{
						this->server->getChannels()[i].secretTopic = (this->splitMessage[2] == "+t") ? true : false;
						for (size_t t = 0; t < this->server->getChannels()[i].clients.size(); t++)
							this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " " + this->splitMessage[2] + "\r\n", this->server->getChannels()[i].clients[t].fd);
						return;
					}
					else if (this->splitMessage[2] == "+l" || this->splitMessage[2] == "-l")
					{
						if (this->splitMessage[2] == "+l")
							this->server->getChannels()[i].limit = atoi(this->splitMessage[3].c_str());
						else
							this->server->getChannels()[i].limit = INT_MAX;
						for (size_t t = 0; t < this->server->getChannels()[i].clients.size(); t++)
							this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " " + this->splitMessage[2] + " " + ((this->splitMessage[2] == "+l") ? this->splitMessage[3] : "") + "\r\n", this->server->getChannels()[i].clients[t].fd);
						return;
					}
					for (size_t t = 0; t < this->server->getChannels()[i].clients.size(); t++)
					{
						if (this->splitMessage[2] == this->server->getChannels()[i].clients[t].nickName)
						{
							if (this->splitMessage[3] == "+o")
							{
								if (this->server->getChannels()[i].clients[t].fd == client->fd)
									return;
								if (!this->server->getChannels()[i].clients[t].isOp)
								{
									this->server->getChannels()[i].clients[t].isOp = true;
									this->server->getChannels()[i].operatorList.push_back(this->server->getChannels()[i].clients[t].fd);
								}
								for (size_t m = 0; m < this->server->getChannels()[i].clients.size(); m++)
									this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " +o " + this->splitMessage[2] + "\r\n", this->server->getChannels()[i].clients[m].fd);
							}
							else if (this->splitMessage[3] == "-o")
							{
								if (this->server->getChannels()[i].operatorList.size() == 1)
									return;
								std::vector<int>::iterator it = std::find(this->server->getChannels()[i].operatorList.begin(), this->server->getChannels()[i].operatorList.end(), this->server->getChannels()[i].clients[t].fd);
								if (it != this->server->getChannels()[i].operatorList.end())
									this->server->getChannels()[i].operatorList.erase(it);
								this->server->getChannels()[i].clients[t].isOp = false;
								for (size_t l = 0; l < this->server->getChannels()[i].clients.size(); l++)
									this->sendMessage(this->formatMessage(*client) + " MODE " + this->splitMessage[1] + " -o " + this->splitMessage[2] + "\r\n", this->server->getChannels()[i].clients[l].fd);
							}
						}
					}
					break;
				}
			}
		}
	}
}

void CommandExecuter::executeTopicCommand()
{
	if (this->splitMessage.size() < 2)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " TOPIC : Usage is TOPIC <#CHANNEL> <TOPIC>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	std::string topicString = "";
	bool		initialTopic = false;
	if (this->splitMessage.size() >= 3)
	{
		for (size_t i = 2; i < this->splitMessage.size(); i++)
		{
			topicString += this->splitMessage[i];
			if (i != this->splitMessage.size() - 1)
				topicString += " ";
		}
		if (!topicString.empty() && topicString[0] == ':')
			topicString = topicString.substr(1);
		initialTopic = true;
	}
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[1] == this->server->getChannels()[i].name)
		{
			for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
			{
				if (client->fd == this->server->getChannels()[i].clients[j].fd)
				{
					if (this->server->getChannels()[i].secretTopic)
					{
						std::vector<int>::iterator	it = std::find(this->server->getChannels()[i].operatorList.begin(), this->server->getChannels()[i].operatorList.end(), client->fd);
						if (it == this->server->getChannels()[i].operatorList.end())
						{
							this->sendMessage(":" + client->hostName + " 482 " + client->nickName + " TOPIC :You're not channel operator\r\n", client->fd);
							return;
						}
					}
					if (initialTopic)
					{
						topicString = topicString.empty() ? "No topic is set" : topicString;
						this->server->getChannels()[i].topic = topicString;
						for (size_t m = 0; m < this->server->getChannels()[i].clients.size(); m++)
							this->sendMessage(this->formatMessage(*client) + " TOPIC " + this->splitMessage[1] + " :" + topicString + "\r\n", this->server->getChannels()[i].clients[m].fd);
					}
					else
					{
						if (this->server->getChannels()[i].topic.empty())
							this->sendMessage(":" + client->hostName + " 331 " + client->nickName + " TOPIC " + this->splitMessage[1] + " :No topic is set\r\n", client->fd);
						else
							this->sendMessage(":" + client->hostName + " 332 " + client->nickName + " TOPIC " + this->splitMessage[1] + " :" + this->server->getChannels()[i].topic + "\r\n", client->fd);
					}

				}
			}
		}
	}
}

void CommandExecuter::executeKickCommand()
{
	if (this->splitMessage.size() < 3)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " KICK : Usage is KICK <#CHANNEL> <NICKNAME>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[1] == this->server->getChannels()[i].name)
		{
			for (size_t k = 0; k < this->server->getChannels()[i].operatorList.size(); k++)
			{
				if (this->server->getChannels()[i].operatorList[k] == client->fd)
				{
					int flag = 0;
					for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
					{
						if (this->splitMessage[2] == this->server->getChannels()[i].clients[j].nickName)
						{
							flag = 1;
							if (this->server->getChannels()[i].clients[j].fd == client->fd)
							{
								this->sendMessage(":" + client->hostName + " 482 " + client->nickName + " KICK :You can't kick yourself\r\n", client->fd);
								return;
							}
							for (size_t k = 0; k < this->server->getChannels()[i].clients.size(); k++)
							{
								this->sendMessage(this->formatMessage(*client) + " KICK " + this->splitMessage[1] + " " + this->splitMessage[2] + "\r\n", this->server->getChannels()[i].clients[k].fd);
							}
							this->server->getChannels()[i].clients.erase(this->server->getChannels()[i].clients.begin() + j);
							if (this->server->getChannels()[i].clients.empty())
								this->server->getChannels().erase(this->server->getChannels().begin() + i);
							return;
						}
					}
					if (!flag)
					{
						this->sendMessage(":" + client->hostName + " 441 " + client->nickName + " KICK :No such nick/channel\r\n", client->fd);
						return;
					}
				}
			}
			this->sendMessage(":" + client->hostName + " 482 " + client->nickName + " KICK :You're not channel operator\r\n", client->fd);
		}
	}
}

void CommandExecuter::executeListCommand()
{
	if (this->splitMessage.size() > 2)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " LIST : Usage is LIST <#CHANNEL>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	if (this->splitMessage.size() == 1)
	{
		for (size_t i = 0; i < this->server->getChannels().size(); i++)
		{
			this->sendMessage(":" + client->hostName + " 322 " + client->nickName + " " + this->server->getChannels()[i].name + " " + (size_tToString(this->server->getChannels()[i].clients.size())) + " :" + this->server->getChannels()[i].topic + "\r\n", client->fd);
		}
	}
	else
	{
		for (size_t i = 0; i < this->server->getChannels().size(); i++)
		{
			if (this->splitMessage[1] == this->server->getChannels()[i].name)
			{
				this->sendMessage(":" + client->hostName + " 322 " + client->nickName + " " + this->server->getChannels()[i].name + " " + (size_tToString(this->server->getChannels()[i].clients.size())) + " :" + this->server->getChannels()[i].topic + "\r\n", client->fd);
				return;
			}
		}
		this->sendMessage(":" + client->hostName + " 403 " + client->nickName + " LIST :No such channel\r\n", client->fd);
	}
}

void CommandExecuter::executeQuitCommand()
{
    if (this->splitMessage.size() < 2)
    {
        this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " QUIT : Usage is QUIT <MESSAGE>. You use too much parameters or your command\r\n", client->fd);
        return;
    }

    std::string message = "";
    for (size_t i = 1; i < this->splitMessage.size(); i++)
    {
        message += this->splitMessage[i];
        if (i != this->splitMessage.size() - 1)
            message += " ";
    }

    std::string quitMessage = ":" + client->nickName + "!" + client->userName + "@" + client->hostName + " QUIT :" + message + "\r\n";

    for (size_t i = 0; i < this->server->getChannels().size(); i++)
    {
        for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
        {
            if (client->fd != this->server->getChannels()[i].clients[j].fd)
            {
                this->sendMessage(quitMessage, this->server->getChannels()[i].clients[j].fd);
            }
        }
    }
    this->splitMessage.clear();
    this->splitMessage.push_back("PART");
    this->splitMessage.push_back("");
    for (size_t i = 0; i < this->server->getChannels().size(); i++)
    {
        for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
        {
            if (client->fd == this->server->getChannels()[i].clients[j].fd)
            {
                this->splitMessage[1] = this->server->getChannels()[i].name;
                this->executePartCommand();
                break;
            }
        }
    }
    close(client->fd);
	for (size_t l = 0; l < this->server->getPollFds().size(); l++)
	{
		if (this->server->getPollFds()[l].fd == client->fd)
		{
			this->server->getPollFds().erase(this->server->getPollFds().begin() + l);
			break;
		}
	}
	for (size_t l = 0; l < this->server->getClients().size(); l++)
	{
		if (this->server->getClients()[l].fd == client->fd)
		{
			this->server->getClients().erase(this->server->getClients().begin() + l);
			break;
		}
	}
}


void CommandExecuter::executeInviteCommand()
{
	if (this->splitMessage.size() < 3)
	{
		this->sendMessage(":" + client->hostName + " 461 " + client->nickName + " INVITE : Usage is INVITE <NICKNAME> <#CHANNEL>. You use too much parameters or your command\r\n", client->fd);
		return;
	}
	for (size_t i = 0; i < this->server->getChannels().size(); i++)
	{
		if (this->splitMessage[2] == this->server->getChannels()[i].name)
		{
			for (size_t j = 0; j < this->server->getChannels()[i].clients.size(); j++)
			{
				if (this->server->getChannels()[i].clients[j].fd == client->fd)
				{
					for (size_t k = 0; k < this->server->getClients().size(); k++)
					{
						if (this->splitMessage[1] == this->server->getClients()[k].nickName)
						{
							this->server->getClients()[k].invChannels.push_back(this->splitMessage[2]);
							this->sendMessage(this->formatMessage(*client) + " INVITE " + this->splitMessage[1] + " " + this->splitMessage[2] + "\r\n", this->server->getClients()[k].fd);
							return;
						}
					}
					this->sendMessage(":" + client->hostName + " 401 " + client->nickName + " INVITE :No such nick/channel\r\n", client->fd);
					return;
				}
			}
		}
	}

}