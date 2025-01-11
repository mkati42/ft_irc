#include "../INCS/main.hpp"

std::string size_tToString(size_t n)
{
    std::stringstream ss;
    ss << n;
    std::string clientSizeStr = ss.str();
    return clientSizeStr;
}

int        charPToInt(const char *str)
{
    std::string s(str);
    
    std::stringstream ss(s);
    int n;
    ss >> n;
    return n;
}

int checkArgs(const char *port, const char *password)
{
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
