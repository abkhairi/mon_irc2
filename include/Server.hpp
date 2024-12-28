/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:02:17 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 23:30:32 by shamsate         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <fcntl.h>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <poll.h>
#include <sys/socket.h>
#include <ctime>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include "Client.hpp"
#include "Channels.hpp"
#include "rep.hpp"


class Client;
class Channels;


class Server
{
    private :
        int                 _fdSockServ;
        int                 _port;
        std::string         _pass;
        std::string         _hostIp;
        std::vector<Client>cliVec;
        std::vector<std::pair<std::string,std::string> > mod;
        Server();
    public :
        Server(int port, std::string pass);
        ~Server();
        size_t              cliIdx;
        std::vector<struct pollfd> pollFdVec;
        std::vector<std::string>   cmdVec;
        std::map<std::string, Channels> channels;
        void init_serv(int port, std::string pass, size_t &i);
        void setFdSockServ(int fd);
        void display();
        int  checkNick(Client& client);
        int     parsNick(Client& client);
        void    removeClient(int id);
        void recvCmd(int fdcli, size_t &idxcli, std::string &cmd);

        void isRegistred(Client &cli, std::string time);
        void rmvCli(int id);
        void sendMsg(Client &cli, std::string vec1, std::string msg);
        void rmvFromCh(int client_fd);

        Client &getCliOrg(int sockCli);
        Client &getCliByIdx(size_t idx);


        std::string getHostIp();
        std::string timee();


        //:::::::::Channel:::::::::::::::

        // void ft_commande_j_m(std::vector<std::string> cmd_final, size_t &_index_client, Client &client_);
        bool find_channel(std::string chan);
        bool isMember(int fdcli, Channels ch);
        void broadCastMsg(Channels ch, std::string msg, int clifd);
        Channels& getChannel(std::string channel);
        void SendToAll(Channels ch, std::string msg);




        int  getFdSockServ();
        //:::::::::CMD:::::::::::::::
        void authCli(std::string cmd, int socket_cli, Client &cli, size_t &idxCli);
        void handleAuthCmd(std::string cmdf, size_t &idxci);
        std::string recvCmd(int fdcli, size_t &idxcli);
        // COMMANDE
        void ft_join(std::vector<std::string> &vec_cmd, Client &client_,size_t &_index_client);
        void kick(std::vector<std::string > vec_cmd, size_t _index_client, Client client_);
        void topic(std::vector<std::string > vec_cmd,size_t _index_client,Client client_);
        void prvMsg(std::vector<std::string > vec_cmd, size_t _indexclient, Client client_);
        void part(std::vector<std::string> veccmd,size_t idxcli,Client cli);
        void eraseCh(std::string _name);
        int  modeSplit(std::string vec_cmd, Client client_);
        void mode(std::vector<std::string > veccmd, size_t idxcli, Client cli);
        void quit(std::vector<std::string > vec_cmd, size_t idxcli, Client cli);
        void invite(std::vector<std::string > vec_cmd, size_t _indexclient, Client client_);
};
void sendMsgToCli(int fdcli, std::string msg);
int  parsingPortPass(std::string port, std::string pass);
void setNonBlocking(int fd);
void    printCh(std::map<std::string, Channels>ch);
void printInfoUser(Channels  &ch);
std::string getListOfNames(std::map<std::pair<bool, int>, Client> _users);
std::string to_lower(std::string str);



#endif