/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:07:24 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:28:16 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"


void    Server::sendMsg(Client &cli, std::string vec1, std::string msg){
    std::string senderNick = cli.getNickNm(); // abkhairi
    std::string senderUsername = cli.getUser(); // a
    if (channels.size() == 0 && vec1[0] == '#'){ // if not have channel{
        sendMsgToCli(cli.getCliFd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, vec1));
        return ;
    }
    std::map<std::string, Channels>::iterator it = channels.find(to_lower(vec1));
    if (it != channels.end())
    {
        if (isMember(cli.getCliFd(), it->second) == false) {// if client not membre in channel o bera iresl msg hhh{
            sendMsgToCli(cli.getCliFd(), ERR_CANNOTSENDTOCHAN(senderNick, it->second.getNmChDispaly()));
            return ;
        }
        // here send msg to all client sauf client li rasel msg
        broadCastMsg(it->second, ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + vec1 + " :" + msg + "\r\n", cli.getCliFd());
    }
    else if (vec1[0] == '#')
    {
        sendMsgToCli(cli.getCliFd(), ERR_NOSUCHCHANNEL(senderNick, senderNick, it->second.getNmCh()));
        return ;
    }
    else
    {
        size_t i = 0;
        while(i < cliVec.size()){
            if (cliVec[i].getNickNm() == vec1)
            {
                sendMsgToCli(cliVec[i].getCliFd(), ":" + senderNick + "!~" + senderUsername + "@127.0.0.1 PRIVMSG " + vec1 + " :" + msg + "\r\n");
                return ;
            }
            i++;
        }
    }
};

void Server::prvMsg(std::vector<std::string > vec_cmd, size_t _indexclient, Client client_){
    std::string msg;
    (void)_indexclient;
    for(size_t i = 0; i < vec_cmd.size(); i++){
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
    sendMsg(client_, vec_cmd[1], msg);
};


