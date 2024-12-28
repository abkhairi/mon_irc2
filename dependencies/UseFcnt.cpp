/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UseFcnt.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhairi <abkhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:02:37 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 17:45:58 by abkhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

int parsingPortPass(std::string port, std::string pass) {
    if (port.empty()) {
        return 1;
    }

    // Use std::string::size_type for the loop index
    for (std::string::size_type i = 0; i < port.size(); i++) {
        if (isdigit(port[i]) == 0) {
            return 1;
        }
    }

    int portNum = atoi(port.c_str());
    if (portNum < 0 || portNum > 65535) {
        return 1;
    }
    if (portNum < 1024) {
        return 1;
    }
    if (pass.empty()) {
        return 2;
    }

    return portNum;
}

void sendMsgToCli(int fdcli, std::string msg){
    ssize_t bytes = send(fdcli, msg.c_str(), msg.size(), 0);
    if (bytes == -1)
        perror("send: ");
};



void setNonBlocking(int fd){
    int flg = fcntl(fd, F_GETFL, 0);
    if (flg == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);}
    if (fcntl(fd, F_SETFL, flg | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
};

std::string getListOfNames(std::map<std::pair<bool, int>, Client> _users)
{
    std::string listOfNames;
    std::map<std::pair<bool, int>, Client>::iterator iter;
    for (iter = _users.begin(); iter != _users.end(); iter++)
    {
        if (iter->first.first == true)
            listOfNames += "@" + iter->second.getNickNm() + " ";
        else
            listOfNames += iter->second.getNickNm() + " ";
    }
    return listOfNames;
}