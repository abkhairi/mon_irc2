#include "../server.hpp"

void    serverr::invite(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_)
{
    size_t indx = 0;
    try 
    {
        channels &obj = getChannel(vec_cmd[2]);
        if(obj.existe_nick(vec_cmd[1]) == true)
            send_msg_to_clinet(client_.get_client_fd(), ERR_USERONCHANNEL(host_ip, client_.get_nickname(), obj.get_name_chanel_display()));
        else if(obj.check_if_operator(client_.get_nickname()) == false)
            send_msg_to_clinet(client_.get_client_fd(), ERR_CHANOPRIVSNEEDED(client_.get_nickname(), obj.get_name_chanel_display()));
        else {
            if(vec_cmd.size() < 3) {
                send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), vec_cmd[2]));
                return ;
            }
            if (!is_membre(client_.get_client_fd(), obj)) {
                send_msg_to_clinet(client_.get_client_fd(), ERR_NOTONCHANNEL(client_.get_nickname(), vec_cmd[2]));
                return ;
            }
            while(indx < vec_client.size())
            {
                if (vec_client[indx].get_nickname() == vec_cmd[1])
                {
                        send_msg_to_clinet(client_.get_client_fd(), RPL_INVITING(host_ip, client_.get_nickname(), vec_cmd[1], vec_cmd[2]));//who invite 
                        send_msg_to_clinet(vec_client[indx].get_client_fd(), RPL_INVITE(client_.get_nickname(), vec_client[indx].get_user(), host_ip, vec_cmd[1], vec_cmd[2]));//c being invited
                        obj.setinvited(vec_client[indx].get_client_fd());
                        break ;
                }
                indx++;
            }
        }
    }
    catch(const char *)
    {
        send_msg_to_clinet(client_.get_client_fd(), ERR_NOSUCHCHANNEL(host_ip, client_.get_nickname(), vec_cmd[2]));
        vec_cmd.clear();
        return ;
    }
}