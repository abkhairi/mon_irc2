#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <sys/socket.h> // Needed for socket creating and binding
#include <netinet/in.h> // Needed to use struct sockaddr_in
#include <fcntl.h> // for fcntl() function for non-blocking socket
#include <vector> // fpr vector
#include <poll.h> // for strcut pollfd
#include <arpa/inet.h>  // For inet_ntoa()
#include <unistd.h> // for close() function
#include <cctype> // for isdigit() function and tolower() function
#include <sstream> // for std::stringstream
#include <cstring> // for std::strstr
#include <ctime>
#include <map>
#include <iomanip> // Nécessaire pour std::setfill et std::setw

#include "client.hpp"
#include "reply_msg.hpp"
#include "channels.hpp"


class cliente;
class channels;

class serverr
{
    private:
        int         _fd_sock_serv;
        int         _port;
        std::string _pass;
        std::string host_ip;
        std::vector <cliente> vec_client;
        std::vector<std::pair<std::string,std::string> > mod;
        serverr();
    public:
        size_t      _index_client;
        std::vector<struct pollfd> vec_pollfd;
        std::map<std::string, channels> channels_;
        std::vector<std::string> vec_of_cmd;
        serverr(int port, std::string pass);
        ~serverr();
        bool    find_channel(std::string chan);
        void    initializer_server(int  port, std::string pass, size_t &i);
        int     get_fd_sock_serv() { return _fd_sock_serv; }
        void    set_fd_sock_serv(int fd) { _fd_sock_serv = fd; }
        cliente& get_client_orgien(int socket_client);
        cliente& get_client_by_index(size_t index);
        std::string get_hostip();
        void    authenticate_client(std::string cmd,int socket_client, cliente &clienteref, size_t &_index_client);
        void    display();
        void is_registre(cliente &client_, std::string time_);
        void receive_cmd(int fd_client, size_t &_index_client, std::string &cmd);
        void handeler_authen_and_commande(std::string cmd_final,size_t &_index_client);
        void ft_commande_j_m(std::vector<std::string> cmd_final, size_t &_index_client, cliente &client_);
        bool is_membre(int fd_client, channels channel_);
        void    broadcastMessage(channels _channel, std::string _message, int _clientfd);
        channels & getChannel(std::string channel);
        void    SendToAll(channels channel_, std::string _message);
        std::string  _time();
        void    remove_Client(int id);
        void    remove_From_Channel(int client_fd);
        void    sendMessage(cliente &client, std::string reciever, std::string _message);
        int     modeSplit(std::string vec_cmd, cliente client_); 
        void    eraseChannel(std::string _name);
        int     checkNick(cliente& client);
        int     parsNick(cliente& client);

        // commande 
        void ft_join(std::vector<std::string> &vec_cmd,cliente &client_,size_t &_index_client);
        void kick(std::vector<std::string > vec_cmd,size_t _index_client, cliente client_);
        void topic(std::vector<std::string > vec_cmd,size_t _index_client,cliente client_);
        void privmsg(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_);
        void quit(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_);
        void invite(std::vector<std::string > vec_cmd, size_t _indexclient, cliente client_);
        void mode(std::vector<std::string > vec_cmd, size_t _index_client, cliente client_);
        void part(std::vector<std::string> vec_cmd,size_t _index_client,cliente client_);

};


int parsing_port_and_pass(std::string port, std::string pass);
void setNonBlocking(int fd) ;

void send_msg_to_clinet(int fd_client, std::string mon_msg);
std::string to_lower(std::string str);
void print_name_channel(cliente client_);
std::string get_str_channels(std::vector<std::string> v);
std::string getListOfNames(std::map<std::pair<bool, int>, cliente> _users);
void print_info_user(channels  &obj);
void print_channel(std::map<std::string, channels> channels_);
#endif