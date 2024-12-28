/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:08:51 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:24:03 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::ft_join(std::vector<std::string> &vec_cmd, Client &client_, size_t &_index_client){
    (void)_index_client;
    
    std::string nickname = client_.getNickNm();
    if (vec_cmd[1].empty())
        sendMsgToCli(client_.getCliFd(), ERR_NEEDMOREPARAMS(nickname, _hostIp));

    std::stringstream split_channel(vec_cmd[1]);
    std::stringstream split_key(vec_cmd[2]);
    std::string name_channel;
    std::string password;
    std::string list_operator;
    std::string hostTheServer = getCliByIdx(_index_client - 1).getIpAddrCli();
    // std::cout << "host the server = " << hostTheServer << std::endl;

    while (std::getline(split_channel, name_channel, ',')){
        std::string name_chan_lower = to_lower(name_channel);
        std::cout << "name channel = " << name_chan_lower << std::endl;
        std::map<std::string, Channels>::iterator it = channels.find(name_chan_lower);
        if (it == channels.end()){// dos not channel so name name_chan_lower
            list_operator = "@"+nickname;
            Channels new_channel(name_chan_lower);
            new_channel.pushToMap(true, client_);// push to map_user
            if (std::getline(split_key, password, ',')){
                new_channel.setPass(password);
                new_channel.setFlgpass(true);
            }
            new_channel.setNmDisplay(name_channel);
            new_channel.setSizeUser(1);
            channels.insert(std::make_pair(name_chan_lower,new_channel));
            sendMsgToCli(client_.getCliFd(), RPL_JOIN(nickname, nickname,name_channel, hostTheServer));
            sendMsgToCli(client_.getCliFd(), RPL_NAMREPLY(hostTheServer, list_operator, name_channel, nickname));
            sendMsgToCli(client_.getCliFd(), RPL_ENDOFNAMES(hostTheServer, nickname, name_channel));
        }
        else if (it != channels.end()) // deja kyna channel
        {
            // check if client into channel
            if (isMember(client_.getCliFd(), it->second) == true){
                std::cout << "Client " << nickname << " is already a member in this channel" << std::endl;
                return ;
            }
            if (it->second.getUserLimit() == true)
            {
                if (it->second.getSizeuser() >= it->second.getLimit()){
                    sendMsgToCli(client_.getCliFd(), "471 " + nickname + " " +  it->second.getNmChDispaly() + " :Cannot join channel, it's full (+l)\r\n");
                    return ;
                }
            }
            if (it->second.getFlgpass() == true){
                std::getline(split_key,password, ',');
                if(it->second.getPass() != password)
                {
                    sendMsgToCli(client_.getCliFd(), ERR_BADCHANNELKEY(nickname, name_channel));
                    return ;
                }
            }
            if (it->second.getInv() == true && it->second.isInvited(client_.getCliFd()) == false){
                //<client> <channel> :Cannot join channel (+i)
                sendMsgToCli(client_.getCliFd(), "473 " + nickname + " " +  it->second.getNmChDispaly() + " :Cannot join channel (+i)\r\n");
                return ;
            }
            it->second.pushToMap(false, client_);
            it->second.setSizeUser(1);
            // it->second.removeInvitedClient(clientfd);
            list_operator = getListOfNames(it->second.getMapUser());
            // send msg to client for entrer to channel
            sendMsgToCli(client_.getCliFd(), RPL_JOIN(nickname, nickname, it->second.getNmChDispaly(), hostTheServer));
            // send msg all client for any client entrer channel 
            broadCastMsg(it->second, RPL_JOIN(nickname, nickname,it->second.getNmChDispaly(), hostTheServer), client_.getCliFd());
            // diaplay list the client in channels par exmple : 353 hicham = #chan1 :idryab hicham @abkhairi
            sendMsgToCli(client_.getCliFd(), RPL_NAMREPLY(hostTheServer, list_operator, it->second.getNmChDispaly(), nickname));
            //Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.
            sendMsgToCli(client_.getCliFd(), RPL_ENDOFNAMES(hostTheServer, nickname, it->second.getNmChDispaly()));
            broadCastMsg(it->second, RPL_ENDOFNAMES(hostTheServer, nickname, it->second.getNmChDispaly()), client_.getCliFd());
            // send msg to client topic the channel = sujet de channel
            sendMsgToCli(client_.getCliFd(), RPL_TOPIC(_hostIp, nickname, it->second.getNmChDispaly(),it->second.getTopic()));
            // send msg to client specifically who set topic (sujet) the channel
            sendMsgToCli(client_.getCliFd(),RPL_TOPICWHOTIME(_hostIp, it->second.getNmChDispaly(), nickname,it->second.getTopicSeter(),client_.getUser(),it->second.getTopicTime()));

        }
    }
    // here push name the channel in vector client : donc chaque client contient vector qui remplir par name des channels qui join
    client_.pushCh(name_channel);
    printCh(channels);
    // print_name_channel(client_); for display all channel the obj client that join
    vec_cmd.clear();
};
