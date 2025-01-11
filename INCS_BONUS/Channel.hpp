#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "main.hpp"

typedef struct s_channel
{
    std::string                 name;
    std::vector<t_client>       clients;
    std::string                 topic;
    std::string                 key;
    size_t                      limit;
	std::vector<int>			operatorList;
    bool                        isSecret;
	bool						secretTopic;
}                               t_channel;

#endif
