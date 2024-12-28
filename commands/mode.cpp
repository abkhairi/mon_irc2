/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:10:41 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:29:48 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int Server::modeSplit(std::string vec_cmd, Client client_){
    // vec_of_cmd = "MODE #chan1 +o nickname +k key";
    //vec_cmd = "+o nickname +k key"
    std::cout << "vec_cmd ===>  " << vec_cmd << std::endl;
    char x;
    std::string res;
    int idex = 3;
    for(int i = 1; vec_cmd[i]; i++) {
        if (vec_cmd[i] != '-' && vec_cmd[i] != '+' && vec_cmd[i] != 'i' 
        && vec_cmd[i] != 't' && vec_cmd[i] != 'o' && vec_cmd[i] != 'k' && vec_cmd[i] != 'l' && vec_cmd[i] != 's' && vec_cmd[i] != 'n') {
            sendMsgToCli(client_.getCliFd(), ERR_UNKNOWNMODE(_hostIp,client_.getNickNm(), vec_cmd[i]));
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
            mod.push_back(std::make_pair(res,cmdVec[idex++]));
        else
            mod.push_back(std::make_pair(res,""));
    }
    return 0;
}

void Server::mode(std::vector<std::string > veccmd, size_t idxcli, Client cli){
    // MODE <channel> <mode> [<parameters>]
    std::string channel;
    std::string channel_name;
    if (veccmd.size() < 2){
        sendMsgToCli(cli.getCliFd(), ERR_NEEDMOREPARAMS(cli.getNickNm(), _hostIp));
        veccmd.clear();
        return ;
    }
    try{
        Channels &obj_chan = getChannel(veccmd[1]);
        channel = cli.getNickNm();
        channel_name = obj_chan.getNmChDispaly();
        if (veccmd.size() > 2)
        {
            if (modeSplit(veccmd[2], cli) == 1)
            {
                mod.clear();
                veccmd.clear();
                return ;
            }
        }
        std::cout << "channel found secssusfly '*`" << std::endl;
        for (size_t i = 0 ; i < mod.size(); i++){
            if (mod[i].first == "+i"){
                if (obj_chan.getInv() == true)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_ALLINV(veccmd[1]));
                else if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else
                {
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "+i"));
                    obj_chan.setInv(true);
                }
            }
            else if (mod[i].first == "-i"){
                if (obj_chan.getInv() == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOTINV(veccmd[1]));
                else if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else {
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "-i"));
                    obj_chan.setInv(false);
                }
            }
            if (mod[i].first == "+t"){
                if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else
                {
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "+t"));
                    obj_chan.setTopicBool(true);
                }
            }
            else if ( mod[i].first == "-t"){
                if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else{
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "-t"));
                    obj_chan.setTopicBool(false);
                }
            }
            if (mod[i].first == "+k"){
                if(veccmd.size() < 4)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOKEY(cli.getNickNm(), channel_name, "k"));
                else if(obj_chan.getPass() == mod[i].second)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_ALREADYSET(cli.getNickNm()));
                else if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else
                {
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "+k " + mod[i].second));
                    obj_chan.setPass(mod[i].second);
                    obj_chan.setFlgpass(true);
                }
            }
            else if (mod[i].first == "-k"){
                if(obj_chan.getFlgpass() == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOPASSSET(cli.getNickNm()));
                else if(obj_chan.getPass() != mod[i].second)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_PASSNOTC(mod[i].second));
                else if (obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else {
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "-k " + mod[i].second));
                    obj_chan.getPass().erase();
                    obj_chan.setFlgpass(false);
            }
            if (mod[i].first == "+o"){
                try {
                    if (veccmd.size() < 4)
                        sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOKEY(cli.getNickNm(), channel_name, "o"));
                    else if(obj_chan.checkIfOperat(channel) == false)
                        sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                    else if(obj_chan.checkIfOperat(mod[i].second) == true)    
                        sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_ALLOP(mod[i].second));
                    else 
                    {
                        obj_chan.getUserBynickname(mod[i].second);
                        obj_chan.setPrvByNickname(mod[i].second, true);
                        SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "+o " + mod[i].second));
                    }
                }    
                catch(const char *){
                        sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_WASNOSUCHNICK(_hostIp,channel));
                }
            }
            else if (mod[i].first == "-o"){
                if(veccmd.size() < 4)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_NEEDMOREPARAM(_hostIp,"-o"));
                else if(obj_chan.checkIfOperat(channel) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_CHANOPRIVSNEEDED(_hostIp,channel_name));
                else if(obj_chan.checkIfOperat(mod[i].second) == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOTOP(mod[i].second));
                else
                {
                    try {
                        obj_chan.getUserBynickname(mod[i].second);
                        obj_chan.setPrvByNickname(mod[i].second, false);
                        SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "-o " + mod[i].second));
                    }
                    catch(const char *){
                        sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), ERR_WASNOSUCHNICK(_hostIp,channel));
                    }
                }
            }
            if (mod[i].first == "+l"){
                if(veccmd.size() < 4){
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOKEY(cli.getNickNm(), channel_name, "l"));
                    veccmd.clear();
                    return ;
                }
                std::stringstream ss(mod[i].second);
                size_t limit;
                ss >> limit;
                if (obj_chan.getLimit() == limit)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_LIMITSET(mod[i].second));
                else
                {
                    obj_chan.setUserLimit(false);
                    obj_chan.setLimit(limit);
                    obj_chan.setUserLimit(true);
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "+l " + mod[i].second));
                }
            }
            else if (mod[i].first == "-l")
            {
                if(obj_chan.getUserLimit() == false)
                    sendMsgToCli(getCliByIdx(idxcli - 1).getCliFd(), RPL_NOLIMITSET(cli.getNickNm()));
                else
                {
                    obj_chan.setUserLimit(false);
                    obj_chan.setLimit(-1);
                    SendToAll(obj_chan, RPL_MODE(obj_chan.getNmChDispaly(), cli.getNickNm(), "-l"));
                }
            }
        }
    }
    }
    catch(const char *) {
        sendMsgToCli(cli.getCliFd(), ERR_NOSUCHCHANNEL(_hostIp, cli.getNickNm(), veccmd[1]));
        mod.clear();
        veccmd.clear();
        return ;
    }
}
