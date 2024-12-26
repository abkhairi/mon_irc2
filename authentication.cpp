#include "server.hpp"
#include "client.hpp"


int serverr::parsNick(cliente& client) 
{
    std::string str = "@&#:1234567890"; //str contient les caractères interdits pour le premier caractère du nickname : @&#:1234567890.
    for (size_t i = 0; i < vec_client.size(); i++) // in loop check if any client has the same nickname
    {
        if(vec_of_cmd[1] == vec_client[i].get_nickname()) 
        {
            client.set_flag_nick(false);
            send_msg_to_clinet(client.get_client_fd(), ERR_NICKNAMEINUSE(vec_of_cmd[1]));
            return 0; // nickname already in use
        }
    }
    if (str.find(vec_of_cmd[1][0]) != std::string::npos)
        client.set_flag_nick(false);
    return 1; // nickname is valid
}

int serverr::checkNick(cliente& client) 
{
    // vec_of_cmd = nick abdo
    std::string str = "@&#:1234567890"; //str contient les caractères interdits pour le premier caractère du nickname : @&#:1234567890.
    std::string s = "_";
    for(size_t i = 0; i < vec_client.size(); i++) // in loop check if any client has the same nickname
    {
        if (vec_of_cmd[1] == vec_client[i].get_nickname()) {
            send_msg_to_clinet(client.get_client_fd(), ERR_NICKNAMEINUSE(vec_of_cmd[1]));
            return 1; // nickname already in use
        }
    }
    if (str.find(vec_of_cmd[1][0]) != std::string::npos)
        return 0; // nickname is invalid :Le premier caractère du surnom est invalide.
    if (s.find(vec_of_cmd[1][strlen(vec_of_cmd[1].c_str()) - 1]) != std::string::npos)
        return 3; // nickname is invalid : Le dernier caractère du surnom est invalide
    return 4; // nickname is valid
}

void serverr::is_registre(cliente &client_, std::string time_)
{
    send_msg_to_clinet(client_.get_client_fd(), RPL_WELCOME(client_.get_nickname(), client_.get_ip_addr_client()));
    send_msg_to_clinet(client_.get_client_fd(), RPL_YOURHOST(client_.get_nickname(), client_.get_ip_addr_client()));
    send_msg_to_clinet(client_.get_client_fd(), RPL_CREATED(client_.get_nickname(), client_.get_ip_addr_client(), time_));
    send_msg_to_clinet(client_.get_client_fd(), RPL_MYINFO(client_.get_nickname(), client_.get_ip_addr_client()));
}


std::string ft_gethostname()
{
    char hostname[256]; // Buffer to store the hostname
    if (gethostname(hostname, sizeof(hostname)) != 0)
        std::cerr << "Error retrieving hostname: " << strerror(errno) << std::endl;
    std::string host_ip = hostname;
    return (host_ip);
}

void serverr::handeler_authen_and_commande(std::string cmd_final,size_t &_index_client)
{
    std::vector<std::string> &vec_of_commande = vec_of_cmd;
    // ft_gethostname();
    char hostname[256]; // Buffer to store the hostname
    if (gethostname(hostname, sizeof(hostname)) != 0)
        std::cerr << "Error retrieving hostname: " << strerror(errno) << std::endl;
    host_ip = hostname;

    cliente &client_ = get_client_by_index(_index_client - 1);
    std::string nick = client_.get_nickname();

    // std::cout << "size of vector = " << vec_of_commande.size() << std::endl;
    // ici segfault if nc localhost 4545 after click sur entre hhhhhh
    if (client_.get_authen() == false)
    {
        if (vec_of_commande.size() <= 1)
        {
            if (vec_of_commande[0] == "nick")
                send_msg_to_clinet(client_.get_client_fd(), ERR_NONICKNAMEGIVEN(nick, host_ip));
            else if (vec_of_commande[0] != "nick" || vec_of_commande.size() == 0)
                send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 461 :Not enough parameters\r\n");
            vec_of_commande.clear();
            return ;
        }
        if (vec_of_commande[0] == "pass" && vec_of_commande[1] == _pass && client_.get_flag_pass() == false)
        {
            if (vec_of_commande.size() == 2)
            {
                // std::cout << "is a pass cmd" << std::endl;
                client_.set_flag_pass(true);
                client_.set_password(vec_of_commande[1]);
            }
            else
                send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 464 :Password incorrect\r\n");
            vec_of_commande.clear();
            return ;
        }
        else if (vec_of_commande[0] == "pass" && vec_of_commande[1] != _pass && client_.get_flag_pass() == false)
        {
            send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 464 :Password incorrect\r\n");
            vec_of_commande.clear();
            return ;
        }
        if (vec_of_commande[0] == "nick" && vec_of_commande.size() == 2 && client_.get_flag_nick() == false)
        {
            // check if any client in vector has the same nickname ft_check_nickname()
            client_.set_flag_nick(true);
            int f = parsNick(client_);
            if (client_.get_flag_nick())
                client_.set_nickname(vec_of_commande[1]);
            else if (f)
                send_msg_to_clinet(client_.get_client_fd(), RPL_ERRONEUSNICKNAME(host_ip, client_.get_nickname()));
            vec_of_commande.clear();
            return ;
        }
        else if (vec_of_commande[0] == "nick" && vec_of_commande.size() > 2 && client_.get_flag_nick() == false)
        {
            send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 432 :Erroneous nickname\r\n");
            vec_of_commande.clear();
            return ;
        }
        if (vec_of_commande[0] == "user" && client_.get_flag_nick() && client_.get_flag_pass())
        {
            // std::cout << "herre fi user\n";
            if (vec_of_commande.size() > 5 || vec_of_commande[1].empty())
            {
                send_msg_to_clinet(client_.get_client_fd(), ":irc.abkhairi.chat 461 :Need more parameters\r\n");
                vec_of_commande.clear();
                return ;
            }
            else
            {
                client_.set_flag_user(true);
                client_.set_user(vec_of_commande[1]);
                client_.set_realname(vec_of_commande[5]);
                std::cout << "user is " << client_.get_user() << std::endl;
                vec_of_commande.clear();
                client_.set_authen();
                time_t currentTime = time(0); // time now non readable for humain
                std::string time_ = ctime(&currentTime);
                is_registre(client_, time_);
                return ;
            }
        }
        vec_of_commande.clear();
        return ;
    }
    else 
    {
        cliente &x = get_client_by_index(_index_client - 1);

        if (vec_of_commande[0] == "pass" || vec_of_commande[0] == "user")
        {
            send_msg_to_clinet(client_.get_client_fd(), ERR_ALREADYREGISTERED(nick, host_ip));
            vec_of_commande.clear();
            return ;
        }
        else if(vec_of_commande[0] == "nick")
        {
            // flag = 1;
            int res = checkNick(get_client_by_index(_index_client-1));// check if deja un client avec le meme nickname
            if (res == 0 || res == 1 || res == 3) 
            {
                if (res == 0) 
                {
                    send_msg_to_clinet(client_.get_client_fd(), RPL_ERRONEUSNICKNAME(host_ip, client_.get_nickname()));
                }
            }
            else
            {
                if (vec_of_commande.size() < 2 || vec_of_commande[1] == ":")
                    send_msg_to_clinet(client_.get_client_fd(), RPL_NONICKNAMEGIVEN(client_.get_nickname(), host_ip));
                else 
                {
                        std::string oldNick = client_.get_nickname();
                        std::vector<std::string> vec_of_name_chan = x.get_chan_name();
                        get_client_by_index(_index_client - 1).set_nickname(vec_of_commande[1]);
                        std::vector<std::string>::iterator it;
                        for(it = vec_of_name_chan.begin(); it != vec_of_name_chan.end(); it++) 
                        {
                            channels &chan = getChannel(*it);
                            chan.updateNickname(oldNick,  chan.check_if_operator(oldNick), get_client_by_index(_index_client - 1));
                            SendToAll(chan, NICKNAME_RPLY(oldNick, client_.get_user(), host_ip, vec_of_commande[1]));
                        }
                        send_msg_to_clinet(client_.get_client_fd(), RPL_NICKCHANGE(oldNick ,vec_of_commande[1] ,host_ip));
                }
            }
        }
        else
        {
            ft_commande_j_m(vec_of_commande, _index_client, client_);
        }
        vec_of_commande.clear();
    }
}




