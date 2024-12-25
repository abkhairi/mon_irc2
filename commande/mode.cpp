#include "../server.hpp"


int serverr::modeSplit(std::string vec_cmd, size_t index, cliente client_) 
{
    // vec_of_cmd = "MODE #chan1 +o nickname +k key";
    //vec_cmd = "+o nickname +k key"
    std::cout << "vec_cmd ===>  " << vec_cmd << std::endl;
    (void)index;
    char x;
    std::string res;
    int idex = 3;
    for(int i = 1; vec_cmd[i]; i++) 
    {
        if (vec_cmd[i] != '-' && vec_cmd[i] != '+' && vec_cmd[i] != 'i' 
        && vec_cmd[i] != 't' && vec_cmd[i] != 'o' && vec_cmd[i] != 'k' && vec_cmd[i] != 'l' && vec_cmd[i] != 's' && vec_cmd[i] != 'n') 
        {
            send_msg_to_clinet(client_.get_client_fd(), ERR_UNKNOWNMODE(host_ip,client_.get_nickname(), vec_cmd[i]));
            return 1;
        }
        if (vec_cmd[i] == '-' || vec_cmd[i] == '+')
            x = vec_cmd[i++];
        if (x == '-') {
            res = "-";
            res += vec_cmd[i];
        }
        else
        {
            res  = "+";
            res += vec_cmd[i];
        }
        if (vec_cmd[i] == 'o' || vec_cmd[i] == 'k' || vec_cmd[i] == 'l')
            mod.push_back(std::make_pair(res,vec_of_cmd[idex++]));
        else
            mod.push_back(std::make_pair(res,""));
    }
    return 0;
}

void serverr::mode(std::vector<std::string > vec_cmd, size_t _index_client, cliente client_)
{
    // MODE <channel> <mode> [<parameters>]
    std::string channel;
    std::string channel_name;
    if (vec_cmd.size() < 2)
    {
        send_msg_to_clinet(client_.get_client_fd(), ERR_NEEDMOREPARAMS(client_.get_nickname(), host_ip));
        vec_cmd.clear();
        return ;
    }
    try
    {
        channels &obj_chan = getChannel(vec_cmd[1]);
        channel = client_.get_nickname();
        channel_name = obj_chan.get_name_chanel_display();
        if (vec_cmd.size() > 2)
        {
            if (modeSplit(vec_cmd[2], _index_client, client_) == 1)
            {
                mod.clear();
                vec_cmd.clear();
                return ;
            }
        }
        std::cout << "channel found secssusfly '*`" << std::endl;
        for (size_t i = 0 ; i < mod.size(); i++)
        {
            if(mod[i].first == "+i")
            {
                std::cout << "mode +i" << std::endl;
            }
            else if(mod[i].first == "-i")
            {
                std::cout << "mode -i" << std::endl;
            }
            if(mod[i].first == "+t")
            {
                std::cout << "mode +t" << std::endl;
            }
            else if( mod[i].first == "-t")
            {
                std::cout << "mode -t" << std::endl;
            }
            if(mod[i].first == "+k")
            {
                std::cout << "mode +k" << std::endl;
            }
            else if(mod[i].first == "-k")
            {
                std::cout << "mode -k" << std::endl;
            }
            if(mod[i].first == "+o")
            {
                std::cout << "mode +o" << std::endl;
            }
            else if(mod[i].first == "-o")
            {
                std::cout << "mode -o" << std::endl;
            }
            if(mod[i].first == "+l")
            {
                std::cout << "mode +l" << std::endl;
            }
            else if(mod[i].first == "-l")
            {
                std::cout << "mode -l" << std::endl;
            }
        }
    }
    catch(const char *) 
    {
        send_msg_to_clinet(client_.get_client_fd(), ERR_NOSUCHCHANNEL(host_ip, client_.get_nickname(), vec_cmd[1]));
        mod.clear();
        vec_cmd.clear();
        return ;
    }
}