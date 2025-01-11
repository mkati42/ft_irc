#include "../INCS_BONUS/IrcServer.hpp"

int main(int ac, char **av)
{
    if (ac == 3)
    {
        switch (checkArgs(av[1], av[2]))
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
            std::cout << "Server started on port " << av[1] << " password is " << av[2] << std::endl;
            IrcServer server(av[1], av[2]);
            break;
        }
    }
    else
    {
        std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
        return 1;
    }
    return 0;
}