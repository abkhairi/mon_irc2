/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhairi <abkhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:01:29 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 17:03:17 by abkhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Client.hpp"

std::string Client::getRecLn(){
    return _dataRec;
};

 Client::Client()
 {
    _pFlg = false;
	_nFlg = false;
	_uFlg = false;
    _auth = false;
    _operator = false;
 }

Client::Client(int sockefd)
{
    _cliSockFd = sockefd;
    _pFlg = false;
	_nFlg = false;
	_uFlg = false;
    _auth = false;
    _operator = false;
}

std::string Client::getIpAddrCli(){
    return (_ipAddrCli);
};

void Client::setRealNm(std::string rname){
    _rname = rname;
};

std::string Client::getRealNm(){
    return (_rname);
}

;void Client::setAuth(){
    _auth = true;
};

void Client::setDataRec(std::string data){
    _dataRec = data;
};

void Client::displayCli(){
        std::cout <<"The Client Fd " << this->_cliSockFd << std::endl;
        std::cout <<"The Client Ip " << this->_ipAddrCli<< std::endl;
};

Client::Client(int clisockfd, std::string ipaddrcli){
    _cliSockFd  = clisockfd;
    _ipAddrCli  = ipaddrcli;
    _auth        = false;
    _nick       = "";
    _CPass   = "";
    _user           = "";
    _pFlg      = false;
    _nFlg      = false;
    _uFlg      = false;
};

int Client::getCliFd(){
    return (_cliSockFd);
};

std::string Client::getNickNm(){
    return (_nick);
}

std::string Client::getPass(){
    return (_CPass);
};

std::string Client::getUser(){
    return (_user);
};

bool Client::getAuth(){
    return (_auth);
};

bool Client::getFlgPass(){
    return (_pFlg);
}
bool Client::getFlgNick(){
    return (_nFlg);
}
bool Client::getFlgUser(){
    return (_uFlg);
}

void Client::setFlgPass(bool flgpass){
    _pFlg = flgpass;
}

void Client::setFlgNick(bool flgnick){
    _nFlg = flgnick;
}

void Client::setFlgUser(bool flguser){
    _uFlg = flguser;
}

void Client::setNickNm(std::string nicknm){
    _nick = nicknm;
}

void Client::setPass(std::string pass){
    _CPass = pass;
}
void Client::setUser(std::string user){
    // _user = user.substr(5, user.size() - 5);
    _user = user;
};

void    Client::pushCh(std::string channel) {
    _channels.push_back(channel);
};
std::vector<std::string > Client::getChNm()
{
    return (_channels);
};
