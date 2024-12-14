#include "../INCS/main.hpp"

std::string size_tToString(size_t n)
{
    std::stringstream ss;
    ss << n;
    std::string clientSizeStr = ss.str();
    return clientSizeStr;
}

int checkArgs(const char *port, const char *password)
{
    for (size_t i = 0; port[i]; i++)
    {
        if (!isdigit(port[i]))
            return 1;
    }
    if (atoi(port) < 1024  || atoi(port) > 49151)
        return 2;
    if (strlen(password) < 6)
        return 3;
    return 0;
}
