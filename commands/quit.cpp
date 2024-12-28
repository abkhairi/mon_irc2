/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:07:39 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 21:12:57 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void    Server::quit(std::vector<std::string > vec_cmd, size_t idxcli, Client cli){
    (void)vec_cmd;
    int clisockfd =  cli.getCliFd();
    std::cout << "Client "<< clisockfd << " disconnected" << std::endl;
    close(clisockfd);
    pollFdVec.erase(pollFdVec.begin() + idxcli);
    rmvCli(idxcli - 1);
    rmvFromCh(clisockfd);
    idxcli--;
    std::map<std::string, Channels>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (it->second.getSizeuser() == 0)
            it = channels.erase(it);
        else
            ++it;
    }
};