#include "../server.hpp"

void serverr::privmsg(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_)
{
    std::string msg;

    for(size_t i = 0; i < vec_cmd.size(); i++)
    {
        if(i > 1)
        {
            if (vec_cmd[2][0] == ':')
                msg += vec_cmd[i] + " ";
            else
            {
                msg = vec_cmd[2];
                break;
            }
        }
    }
    if (msg[0] == ':')
        msg = msg.substr(1);
    sendMessage(client_, vec_cmd[1], msg);
}

void    serverr::sendMessage(cliente &client, std::string vec1, std::string _message)
{
    std::string senderNick = client.get_nickname(); // abkhairi
    std::string senderUsername = client.get_user(); // a
    if (channels_.size() == 0 && vec1[0] == '#') // if not have channel 
    {
        send_msg_to_clinet(client.get_client_fd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, vec1));
        return ;
    }
    std::map<std::string, channels>::iterator it = channels_.find(to_lower(vec1));
    if (it != channels_.end())
    {
        if (is_membre(client.get_client_fd(), it->second) == false) // if client not membre in channel o bera iresl msg hhh
        {
            send_msg_to_clinet(client.get_client_fd(), ERR_CANNOTSENDTOCHAN(senderNick, it->second.get_name_chanel_display()));
            return ;
        }
        // here send msg to all client sauf client li rasel msg
        broadcastMessage(it->second, ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + vec1 + " :" + _message + "\r\n", client.get_client_fd());
    }
    else if (vec1[0] == '#')
    {
        send_msg_to_clinet(client.get_client_fd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, it->second.get_name_chan()));
        return ;
    }
    else
    {
        size_t i = 0;
        while(i < vec_client.size())
        {
            if (vec_client[i].get_nickname() == vec1)
            {
                send_msg_to_clinet(vec_client[i].get_client_fd(), ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + vec1 + " :" + _message + "\r\n");
                return ;
            }
            i++;
        }
    }
}
