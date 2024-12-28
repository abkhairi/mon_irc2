/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Authentification.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:02:10 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:32:15 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include "../include/Channels.hpp"

std::string getHostNm(){
    char    hostnm[256];
    if (gethostname(hostnm, sizeof(hostnm)) != 0)
        std::cerr<< " Error retrieving hostname: " << strerror(errno) << std::endl;
    std::string hostip = hostnm;
    return (hostip);
};


void Server::handleAuthCmd(std::string cmdf, size_t &idxcli)
{
    std::vector<std::string> &cmdvec = cmdVec;
    // ft_gethostname();
    char hostnm[256]; // Buffer to store the hostname
    (void)cmdf;
    if (gethostname(hostnm, sizeof(hostnm)) != 0)
        std::cerr << "Error retrieving hostname: " << strerror(errno) << std::endl;
    _hostIp = hostnm;

    Client &cli = getCliByIdx(idxcli - 1);
    std::string nick = cli.getNickNm();

    // std::cout << "size of vector = " << vec_of_commande.size() << std::endl;
    // ici segfault if nc localhost 4545 after click sur entre hhhhhh
    std::string cmd = to_lower(cmdvec[0]);
    if (cli.getAuth() == false)
    {
        if (cmdvec.size() <= 1)
        {
            if (cmdvec[0] == "nick")
                sendMsgToCli(cli.getCliFd(), ERR_NONICKNAMEGIVEN(nick, _hostIp));
            else if (cmdvec[0] != "nick" || cmdvec.size() == 0)
                sendMsgToCli(cli.getCliFd(), ":IRC ==> 461 :Not enough parameters\r\n");
            cmdvec.clear();
            return ;
        }
        if (cmdvec[0] == "pass" && cmdvec[1] == _pass && cli.getFlgPass() == false)
        {
            if (cmdvec.size() == 2)
            {
                // std::cout << "is a pass cmd" << std::endl;
                cli.setFlgPass(true);
                cli.setPass(cmdvec[1]);
            }
            else
                sendMsgToCli(cli.getCliFd(), ":IRC ==> 464 :Password incorrect\r\n");
            cmdvec.clear();
            return ;
        }
        else if (cmdvec[0] == "pass" && cmdvec[1] != _pass && cli.getFlgPass() == false)
        {
            sendMsgToCli(cli.getCliFd(), ":IRC ==> 464 :Password incorrect\r\n");
            cmdvec.clear();
            return ;
        }
        if (cmdvec[0] == "nick" && cmdvec.size() == 2 && cli.getFlgNick() == false && cli.getFlgPass() == true)
        {
            // check if any client in vector has the same nickname ft_check_nickname()
            cli.setFlgNick(true);
            int x = parsNick(cli);
            if(cli.getFlgNick())
                cli.setNickNm(cmdVec[1]);
            else if(x)
                sendMsgToCli(cli.getCliFd(), RPL_ERRONEUSNICKNAME(_hostIp, nick));
            cli.setFlgNick(true);
            cli.setNickNm(cmdvec[1]);
            cmdvec.clear();
            return ;
        }
        else if (cmdvec[0] == "nick" && cmdvec.size() > 2 && cli.getFlgNick() == false)
        {
            sendMsgToCli(cli.getCliFd(), ":IRC ==> 432 :Erroneous nickname\r\n");
            cmdvec.clear();
            return ;
        }
        if (cmdVec[0] == "user" && cli.getFlgNick() && cli.getFlgPass())
        {
            // std::cout << "herre fi user\n";
            if (cmdvec.size() != 5 || cmdvec[1].empty())
            {
                sendMsgToCli(cli.getCliFd(), ":irc.abkhairi.chat 461 :Need more parameters\r\n");
                cmdvec.clear();
                return ;
            }
            else
            {
                cli.setFlgUser(true);
                cli.setUser(cmdvec[1]);
                cli.setRealNm(cmdvec[5]);
                std::cout << "user is " << cli.getUser() << std::endl;
                cmdvec.clear();
                cli.setAuth();
                time_t currentTime = time(0); // time now non readable for humain
                std::string time_ = ctime(&currentTime);
                isRegistred(cli, time_);
                return ;
            }
        }
        cmdvec.clear();
        return ;
    }
    else
    {
        if (cmdvec[0] == "pass" || cmdvec[0] == "user")
        {
            sendMsgToCli(cli.getCliFd(), ERR_ALREADYREGISTERED(nick, _hostIp));
            cmdvec.clear();
            return ;
        }
        else if (cmdvec[0] == "nick")
        {
            // change nick the user
            // flag = 1;
            Client &x = getCliByIdx(cliIdx - 1);
            int res = checkNick(getCliByIdx(cliIdx -1));// check if deja un client avec le meme nickname
            std::cout << "res = " << res<<std::endl;
            if (res == 0 || res == 1 || res == 3) 
            {
                if (res == 0) 
                {
                    sendMsgToCli(cli.getCliFd(), RPL_ERRONEUSNICKNAME(_hostIp, cli.getNickNm()));
                }
            }
            else
            {
                if (cmdVec.size() < 2 || cmdVec[1] == ":")
                    sendMsgToCli(cli.getCliFd(), RPL_NONICKNAMEGIVEN(cli.getNickNm(), _hostIp));
                else 
                {
                    std::string oldNick = cli.getNickNm();
                    std::vector<std::string> vec_of_name_chan = x.getChNm();
                    getCliByIdx(cliIdx - 1).setNickNm(cmdVec[1]);
                    std::vector<std::string>::iterator it;
                    for(it = vec_of_name_chan.begin(); it != vec_of_name_chan.end(); it++) 
                    {
                        Channels &chan = getChannel(*it);
                        chan.updateNickname(oldNick,  chan.checkIfOperator(oldNick), getCliByIdx(cliIdx - 1));
                        SendToAll(chan, NICKNAME_RPLY(oldNick, cli.getUser(), _hostIp, cmdVec[1]));
                    }
                    sendMsgToCli(cli.getCliFd(), RPL_NICKCHANGE(oldNick ,cmdVec[1] ,_hostIp));
                }
            }
        }
        if (cmd == "join")
        {
            if (cmdvec.size() < 2)
                sendMsgToCli(cli.getCliFd(), ERR_NEEDMOREPARAMS(cli.getNickNm(), _hostIp));
            else
                ft_join(cmdvec, cli, idxcli);
        }
        else if (cmd == "kick")
        {
            kick(cmdvec, idxcli, cli);
        }
        else if (cmd == "topic")
        {
            topic(cmdvec, idxcli, cli);
        }
        else if (cmd == "privmsg")
        {
            prvMsg(cmdvec, idxcli, cli);
        }
        else if (cmd == "privmsg")
        {
            prvMsg(cmdvec, idxcli, cli);
        }
        else if (cmd == "quit")
        {
            quit(cmdvec, idxcli, cli);
        }
        else if (cmd == "invite")
        {
            invite(cmdvec, idxcli, cli);
        }
        else if (cmd == "mode")
        {
            mode(cmdvec, idxcli, cli);
        }
        else if (cmd == "part")
        {
            part(cmdvec, idxcli, cli);
        }
        else
        {
            sendMsgToCli(cli.getCliFd(), ERR_UNKNOWNCOMMAND(_hostIp, cmdVec[0]));
        }
        cmdvec.clear();
        mod.clear();
    }
}






