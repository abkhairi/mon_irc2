#include "../server.hpp"


void    serverr::quit(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_)
{
    int client_sock_fd =  client_.get_client_fd();
    std::cout << "Client "<< client_sock_fd << " disconnected" << std::endl;
    close(client_sock_fd);
    vec_pollfd.erase(vec_pollfd.begin() + _index_client);
    remove_Client(_index_client - 1);
    remove_From_Channel(client_sock_fd);
    _index_client--;
     std::map<std::string, channels>::iterator it = channels_.begin();
    while (it != channels_.end())
    {
        if (it->second.get_size_user() == 0)
            it = channels_.erase(it);
        else
            ++it;
    }
}