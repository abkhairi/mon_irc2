/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:07:34 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 21:13:12 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::topic(std::vector<std::string > vec_cmd,size_t _index_client,Client client_){
    (void)_index_client;
    try
    {
        if (!client_.getAuth())
        {
            sendMsgToCli(client_.getCliFd(), ERR_NOTREGISTERED(client_.getNickNm(), _hostIp));
            return ;
        }
        if (vec_cmd.size() < 2)
        {
            sendMsgToCli(client_.getCliFd(), ERR_NEEDMOREPARAMS(client_.getNickNm(), _hostIp));
            return;
        }
        // check if channel exicte or no
        if (!find_channel(vec_cmd[1]))
        {
                sendMsgToCli(client_.getCliFd(),ERR_NOSUCHCHANNEL(_hostIp, vec_cmd[1], client_.getNickNm()));
                return ;
        }
        Channels &channel_ = getChannel(vec_cmd[1]);
        if(vec_cmd.size() == 2)
        {
            if (channel_.getTopicBool() == false)
                sendMsgToCli(client_.getCliFd(),RPL_NOTOPIC(_hostIp, client_.getNickNm(),channel_.getNmChDispaly()));
            else
            {
                sendMsgToCli(client_.getCliFd(),RPL_TOPICDISPLAY(_hostIp, client_.getNickNm(),channel_.getNmChDispaly(),channel_.getTopic()));
                sendMsgToCli(client_.getCliFd(),RPL_TOPICWHOTIME(_hostIp ,channel_.getNmChDispaly(), client_.getNickNm(),channel_.getTopicSeter(),client_.getUser(),channel_.getTopicTime()));
            }
        }
        if (vec_cmd.size() >= 3)
        {
                std::string res; // res c est le sujet de channel
                for (size_t i = 2; i < vec_cmd.size(); i++)
                {
                    res += vec_cmd[i];
                    if (i != vec_cmd.size())
                        res = res + " ";
                }
                std::cout << "|" << res << "|" << std::endl;
                channel_.setTopicAttr(res, true, client_.getNickNm(), timee());
                if (channel_.nickExist(client_.getNickNm()) == false)
                    sendMsgToCli(client_.getCliFd(), ERR_NOTONCHANNEL(_hostIp,client_.getNickNm()));
                else if (channel_.checkIfOperat(client_.getNickNm()) == false)
                    sendMsgToCli(client_.getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,client_.getNickNm()));
                else
                    SendToAll(channel_, RPL_NEWTOPICSETTED(client_.getNickNm(), channel_.getTopicSeter(), _hostIp, channel_.getNmChDispaly(),res));
        }
    }
    catch(const char *str)
    {
            sendMsgToCli(client_.getCliFd(), ERR_NOSUCHCHANNEL(_hostIp, client_.getNickNm(), vec_cmd[1]));
    }
};

