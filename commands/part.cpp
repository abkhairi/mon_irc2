/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:07:19 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:27:25 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::part(std::vector<std::string> veccmd,size_t idxcli,Client cli){
    (void)idxcli;
    try{
        rmvFromCh(cli.getCliFd());
        Channels &obj = getChannel(cmdVec[1]);
        if (obj.getSizeuser() == 0)
            eraseCh(to_lower(cmdVec[1]));
        sendMsgToCli(cli.getCliFd(), RPL_QUIT(cli.getNickNm(),_hostIp, "good bye"));
        sendMsgToCli(cli.getCliFd(), RPL_PART(cli.getNickNm(),cli.getUser(), _hostIp, cmdVec[1]));
    }
    catch(const char *){
        sendMsgToCli(cli.getCliFd(), ERR_NOSUCHCHANNEL(_hostIp, cli.getNickNm(), cmdVec[1]));
        veccmd.clear();
        return ;
    }
};