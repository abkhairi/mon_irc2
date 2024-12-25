#include "../server.hpp"


void serverr::part(std::vector<std::string> vec_cmd,size_t _index_client,cliente client_)
{
    try
    {
        remove_From_Channel(client_.get_client_fd());
        channels &obj = getChannel(vec_cmd[1]);
        if (obj.get_size_user() == 0)
            eraseChannel(to_lower(vec_cmd[1]));
        send_msg_to_clinet(client_.get_client_fd(), RPL_QUIT(client_.get_nickname(),host_ip, "good bye"));
        send_msg_to_clinet(client_.get_client_fd(), RPL_PART(client_.get_nickname(),client_.get_user(), host_ip, vec_cmd[1]));
    }
    catch(const char *)
    {
        send_msg_to_clinet(client_.get_client_fd(), ERR_NOSUCHCHANNEL(host_ip, client_.get_nickname(), vec_cmd[1]));
        vec_of_cmd.clear();
        return ;
    }
}