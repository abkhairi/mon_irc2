/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 20:58:54 by r4v3n             #+#    #+#             */
/*   Updated: 2024/12/26 20:31:14 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Server.hpp"

bool   Server::find_channel(std::string chan)
{
    std::map<std::string, Channels>::iterator it = channels.begin();
    for (it = channels.begin(); it !=  channels.end(); it++)
	{
            if (to_lower(it->first) == to_lower(chan))
            {
                std::cout << "true and name channel is = " << (it->first) << std::endl;
                return true;
            }
	}
	return false;
}

int Server::parsNick(Client& client)
{
    std::string str = "@&#:1234567890";
    for(size_t i = 0; i < cliVec.size(); i++) {
        if(cmdVec[1] == cliVec[i].getNickNm()) {
            client.setFlgNick(false);
            sendMsgToCli(client.getCliFd(), ERR_NICKNAMEINUSE(cmdVec[1]));
            return 0;
        }
    }
    if(str.find(cmdVec[1][0]) != std::string::npos)
        client.setFlgNick(false);
    return 1;
}

int Server::checkNick(Client& client)
{
    // vec_of_cmd = nick abdo
    std::string str = "@&#:1234567890"; //str contient les caractères interdits pour le premier caractère du nickname : @&#:1234567890.
    std::string s = "_";
    for(size_t i = 0; i < cliVec.size(); i++) // in loop check if any client has the same nickname
    {
        if (cmdVec[1] == cliVec[i].getNickNm()) {
            sendMsgToCli(client.getCliFd(), ERR_NICKNAMEINUSE(cmdVec[1]));
            return 1; // nickname already in use
        }
    }
    if (str.find(cmdVec[1][0]) != std::string::npos)
        return 0; // nickname is invalid :Le premier caractère du surnom est invalide.
    if (s.find(cmdVec[1][strlen(cmdVec[1].c_str()) - 1]) != std::string::npos)
        return 3; // nickname is invalid : Le dernier caractère du surnom est invalide
    return 4; // nickname is valid
}

Server::Server(int port, std::string pass){
    _port = port;
    _pass = pass;
};

Server::~Server(){};

Client& Server::getCliByIdx(size_t idx){
    return (cliVec[idx]);
};
std::string Server::getHostIp(){
    return (_hostIp);
};
void    Server::display(){
    for (std::vector<struct pollfd>::iterator it = pollFdVec.begin(); it != pollFdVec.end(); it++)
    {
        std::cout << "Struct  Fd  = " << it->fd << std::endl;
        std::cout << "Struct Event  = " << it->events << std::endl;
        std::cout << "Struct Revent  = " << it->revents << std::endl;
    }
};
void  Server::rmvCli(int id){
    cliVec.erase(cliVec.begin() +id);
};
std::string Server::timee() {
    time_t now = time(0);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer) + " (" + std::to_string(now) + ")";
};
Client & Server::getCliOrg(int sockcli){
    for (size_t i = 0 ; cliVec.size() > i; i++){
        if (cliVec[i].getCliFd() == sockcli)
            return(cliVec[i]);
    }
    return (cliVec[0]);
};
int  Server::getFdSockServ(){
    return (_fdSockServ);
};

void    Server::authCli(std::string cmd, int socket_client, Client &clienteref, size_t &_index_client)
{
    (void)socket_client;
    for (size_t i = 0; i < cmd.size(); i++)
        cmd[i] = std::tolower(cmd[i]);
    // pass abc
    std::stringstream ss(cmd);
    // int i = 0;
    while (ss >> cmd)
    {
        cmdVec.push_back(cmd);
        // i++;
    }
    if (strstr(clienteref.getRecLn().c_str(), "\n"))
    {
        size_t position = clienteref.getRecLn().find_first_of("\n");
        if (position > clienteref.getRecLn().size())
            return;
        // std::cout << "position = " << position << std::endl;
        std::string cmd_final = clienteref.getRecLn().substr(0 , position + 1);
        // std::cout << "cmd_final = " << cmd_final << std::endl;
        handleAuthCmd(cmd_final, _index_client);
    }
    else
        return ;
};

void    Server::init_serv(int  port, std::string pass, size_t &i){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr <<"Error:  failed to create socket" << std::endl;
        exit(1);
    }

    int opt = 1;// setsockopt : function in network programming is used to configure options on a socket.
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr <<"Error: setsockopt() failed" << std::endl;
        close(sockfd);
        exit(1);
    }
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
    {
        perror("fcntl");
        close(sockfd);
        exit(EXIT_FAILURE);
    }    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) // bound [ip and port] to server socket
    {
        std::cerr <<"Error: bind failed" << std::endl;
        close(sockfd);
        exit(1);
    }
    if (listen(sockfd, 10) == -1)
    {
        std::cerr <<"Error: listen failed" << std::endl;
        close(sockfd);
        exit(1);
    }
    std::cout << "\033[32m+:::::::::[FT_IRC]:::::::::+\033[0m" << std::endl <<"\033[32m+\033[0m";
    std::cout << "\033[31m The Server listen in ==> " <<"\033[32m+" << std::endl;
    std::cout << "\033[32m+ Port :\033[0m              " << port << "\033[32m +\033[0m"<< std::endl;
    std::cout << "\033[32m+ Pass :\033[0m              " << pass << "\033[32m +\033[0m"<<std::endl;
    std::cout << "\033[32m++++++++++++++++++++++++++++\033[0m" << std::endl;
    setFdSockServ(sockfd);
    struct pollfd poolfd;
    poolfd.fd = getFdSockServ();
    poolfd.events = POLLIN;
    pollFdVec.push_back(poolfd);
    while (1){
        int res = poll(pollFdVec.data(), pollFdVec.size(), -1);
        if (res == -1){
            std::cout << "error poll\n";
            return ;}
        for (i = 0; pollFdVec.size() > i; i++){
            if (POLLIN & pollFdVec[i].revents){
                if (pollFdVec[i].fd == getFdSockServ()){
                    // is a server here : is a handle new connction for client
                    struct sockaddr_in cli_addr;
                    socklen_t len = sizeof(cli_addr);
                    int cli_fd = accept(getFdSockServ(), (struct sockaddr*)&cli_addr, &len);
                    if (cli_fd == -1)
                        perror("accept");
                    // setNonBlocking(cli_fd);
                    struct pollfd poollfd;
                        poollfd.fd = cli_fd;
                        poollfd.events = POLLIN | POLLOUT;
                        poollfd.revents = 0;
                    pollFdVec.push_back(poollfd);
                    std::string ipAddrCli = inet_ntoa(cli_addr.sin_addr);
                    std::cout << "display ip addres client = " << ipAddrCli << std::endl;
                    Client obj_client(cli_fd, ipAddrCli);
                    obj_client.setipaddr(ipAddrCli);
                    cliVec.push_back(obj_client);
                    std::cout << "New connection accepted: " << cli_fd << std::endl;
                }
                else
                {
                    int sockcli = pollFdVec[i].fd;
                    std::string cmd ;
                    recvCmd(sockcli, i, cmd);
                    Client &cliref = getCliOrg(sockcli);
                    cliref.setDataRec(cmd);
                    authCli(cmd, sockcli, cliref, i);
                }
            }
        }
    }
};

void Server::setFdSockServ(int fd){
    _fdSockServ = fd;
};

void Server::isRegistred(Client &cli, std::string time){
    sendMsgToCli(cli.getCliFd(),RPL_WELCOME(cli.getNickNm(), cli.getIpAddrCli()));
    sendMsgToCli(cli.getCliFd(),RPL_YOURHOST(cli.getNickNm(), cli.getIpAddrCli()));
    sendMsgToCli(cli.getCliFd(),RPL_CREATED(cli.getNickNm(), cli.getIpAddrCli(), time));
    sendMsgToCli(cli.getCliFd(),RPL_MYINFO(cli.getNickNm(), cli.getIpAddrCli()));
};

void    Server::removeClient(int id)
{
    cliVec.erase(cliVec.begin() +id);
}

void Server::recvCmd(int fdcli, size_t &idxcli, std::string &cmd){
    char buffer[1024];
    memset(buffer, 0, 1024);
    ssize_t bytes_received = recv(fdcli, buffer, sizeof(buffer), 0);
    if(bytes_received == -1)
        perror("recv");
    else if (bytes_received == 0)
    {
        std::cout << "Client disconnected: " << fdcli << std::endl;
        close(fdcli); // close the socket if not present program infinite loop infoi click sur c
        pollFdVec.erase(pollFdVec.begin() + idxcli);
        removeClient(idxcli - 1);
        rmvFromCh(fdcli);
        idxcli--;
        std::map<std::string, Channels>::iterator it = channels.begin();
        while (it != channels.end())
        {
            if (it->second.getSizeuser() == 0)
                it = channels.erase(it);
            else
                ++it;
        }
        return ;
    }
    else
    {
        buffer[bytes_received] = '\0'; // add null terminator if not present => display garbej value
        cmd = buffer;
    }
};

std::string to_lower(std::string str){
    for (size_t i =0; str.size() > i; i++)
        str[i] = tolower(str[i]);
    return (str);
};

Channels & Server::getChannel(std::string channel){
    std::map<std::string, Channels>::iterator it = channels.find(to_lower(channel));
    if (it == channels.end())
        throw "No channel found";
    return it->second;
};

void    Server::SendToAll(Channels ch, std::string msg){
    std::map<std::pair<bool, int>, Client> mapOfClients = ch.getMapUser();
    std::map<std::pair<bool, int>, Client>::iterator iter;
    for(iter = mapOfClients.begin(); iter != mapOfClients.end(); iter++)
        sendMsgToCli(iter->second.getCliFd(), msg);
        // send_msg_to_clinet(iter->first.second, _message);
};

void Server::rmvFromCh(int client_fd){
    std::map<std::string, Channels>::iterator it = channels.begin();

    for (; it != channels.end(); it++)
    {
        std::map<std::pair<bool, int>, Client> &users_map = it->second.getMapUser();
        std::map<std::pair<bool, int>, Client>::iterator iter = users_map.begin();
        for (;iter != users_map.end(); ++iter)
        {
            if(client_fd == iter->second.getCliFd())
            {
                broadCastMsg(it->second, RPL_QUIT(iter->second.getNickNm(), _hostIp, "good bye"), client_fd);
                users_map.erase(iter);
                it->second.setSizeUser(-1);
                break ;
            }
        }
    }
}

void Server::eraseCh(std::string _name){
    channels.erase(_name);
};
