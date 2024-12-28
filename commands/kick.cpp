/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:08:46 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:22:57 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::kick(std::vector<std::string > vec_cmd, size_t _index_client, Client client_){
    (void)_index_client;
    // (void)client_;
    std::stringstream users(vec_cmd[2]);
    std::string user;
    std::string commntaire;

    // std::string nick = client_.get_nickname();
    while (std::getline(users,user, ',')) {
        try{
            if (client_.getAuth() == false)
            {
                sendMsgToCli(client_.getCliFd(),ERR_NOTREGISTERED(client_.getNickNm(), _hostIp));
                return ;
            }
            if (vec_cmd.size() < 3) {
                sendMsgToCli(client_.getCliFd(),ERR_NEEDMOREPARAMS(client_.getNickNm(), _hostIp));
                return ;
            }
            Channels &obj_chan = getChannel(vec_cmd[1]);
            std::string str = vec_cmd[1];
            // commantaire c est a partir de : commaintaire 
            if (vec_cmd.size() == 3 || (vec_cmd.size() == 4 && vec_cmd[3] == ":"))
                commntaire = ":no respect rule";
            else 
            {
                for (size_t i = 3; i < vec_cmd.size(); i++){
                    commntaire = commntaire + vec_cmd[i];
                    if (i < vec_cmd.size()) 
                        commntaire = commntaire + " ";
                }
            }
            if (str[0] == '#' && str.length() > 1)
                vec_cmd[0] = vec_cmd[0].substr(1);
            else 
            {
                sendMsgToCli(client_.getCliFd(),ERR_NOSUCHCHANNEL(_hostIp, str, client_.getNickNm()));
                return ;
            }
            if (!(find_channel(vec_cmd[1]))) {
                sendMsgToCli(client_.getCliFd(),ERR_NOSUCHCHANNEL(_hostIp, str, client_.getNickNm()));
                return ;
            }
            if (obj_chan.checkIfOperat(client_.getNickNm()) == false) {
                sendMsgToCli(client_.getCliFd(),ERR_NOTOP(_hostIp, str));
                return ;
            }
            if (obj_chan.nickExist(user) == false){
                sendMsgToCli(client_.getCliFd(), ERR_NOSUCHNICK(_hostIp, str, vec_cmd[2]));
                return;
            }
            // if (check_if_nick_in_chan(vec[1], vec[2]) == false) {
            // 	sendTo(ERR_USERNOTINCHANNEL(Server::_hostname, vec[1]));
            // 	return ;
            // }
            else
            {
                SendToAll(obj_chan, RPL_KICK(client_.getNickNm(), client_.getUser(), _hostIp, user, vec_cmd[1], commntaire));
                obj_chan.deleteCli(user);
                obj_chan.setSizeUser(-1);
            }
        }
        catch(const char *)  //Catch const char* exception without using the value
        {
        sendMsgToCli(client_.getCliFd(), ERR_NOSUCHCHANNEL(_hostIp, vec_cmd[1], client_.getNickNm()));
    }
    };
}
