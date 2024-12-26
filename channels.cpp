#include "channels.hpp"

void channels::updateNickname(std::string oldnick, bool prv, cliente &obj) 
{    
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();
    for (; it != _users.end(); ++it) 
    {
        if (it->second.get_nickname() == oldnick) 
        {
            int x = it->second.get_client_fd();
            std::pair<bool, int> newKey(prv, x);
            _users[it->first] = obj;
            break; 
        }
    }
}

void	channels::setLimit(size_t limit)
{
    _limit = limit;
}

bool	channels::getUserLimit()
{
    return _userlimit;
}

void	channels::setUserLimit(bool limit)
{
    _userlimit = limit;
}

void channels::setPass(bool pass)
{
    _pass = pass;
}
void channels::set_topic_bool(bool topc)
{
    _topc = topc;
}

channels::channels(std::string name_channel)
{
    _name = name_channel;
    _inv = false;
    _flag_pass = false;
    _topc = false;
    _limit = -1;
    _usersSize = 0;
}

bool channels::get_topic_bool()
{
    return (_topc);
}

void    channels::setinvited(int sockfd)
{
    _invited.push_back(sockfd);
}

std::string     channels::gettopicseter() {
    return topicseter;
}

std::string     channels::gettopictime() {
    return topictime;
}


void    channels::setTopicAttr(std::string topic, bool topc, std::string seter, std::string time) {
    topicseter = seter;
    topictime = time;
    _topic = topic;
    _topc = topc;
}


std::string     channels::getTopic() 
{
    return _topic;
}

void     channels::setTopic(std::string topic) 
{
     _topic = topic;
}

bool	channels::isInvited(int sockfd)
{
	std::vector<int>::iterator it = std::find(_invited.begin(), _invited.end(), sockfd);
    if(it != _invited.end())
    {
        return true;
    }
    return false;
}


bool channels::get_inv()
{
    return (_inv);
}

void channels::set_inv(bool in)
{
    _inv = in;
}

bool channels::get_si_user_limit()
{
    return (_userlimit);
}

size_t channels::get_size_user()
{
    return (_usersSize);
}

size_t channels::get_limit()
{
    return (_limit);
}

std::string channels::get_name_chanel_display()
{
    return (name_channel_display);
}

void channels::set_name_display(std::string name)
{
    name_channel_display = name;
}


void channels::push_to_map(bool privilege, cliente &client_)
{
    std::pair<bool, int> paire(privilege, client_.get_client_fd());
	_users.insert(std::make_pair(paire, client_));
}

std::string channels::get_name_chan()
{
    return (_name);
}

std::string& channels::get_password()
{
    return (_password);
}

std::map<std::pair<bool,int>, cliente >& channels::get_map_user()
{
	return _users;
}


void channels::set_size_users(size_t size_user)
{
    _usersSize = size_user + _usersSize; 
}

bool channels::get_flag_pass()
{
    return (_flag_pass);
}
void channels::set_password(std::string password)
{
    _password = password;
}

void channels::set_flagpass(bool flagpass)
{
    _flag_pass = flagpass;
}

bool   channels::getPass()
{
    return (_pass);
}

bool    channels::check_if_operator(std::string nickname)
{
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();

    for(; it != _users.end(); it++) 
    {
        if(it->second.get_nickname() == nickname)
            return it->first.first;
    }
    throw "bool not found";
}

bool channels::existe_nick(std::string user)
{
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();
    for (it = _users.begin(); it != _users.end(); it++)
    {
        if (it->second.get_nickname() == user)
            return (true);
    }
    return (false);
}

void channels::deletClient(std::string nick)
 {
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();
    for (it = _users.begin(); it != _users.end(); it++) 
    {
        if (it->second.get_nickname() == nick)
            _users.erase(it++);
    }
}


cliente&		channels::getUserBynickname(std::string nickname)
{
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();
    for(; it != _users.end(); it++) {
        if(it->second.get_nickname() == nickname) {
            return it->second;
        }
    }
    throw "User not found";
}

void channels::setPrvByNickname(std::string nickname, bool prv) {
    
    std::map<std::pair<bool, int>, cliente>::iterator it = _users.begin();
    bool found = false;
    for (; it != _users.end(); ++it) 
    {
        if (it->second.get_nickname() == nickname) 
        {
            cliente tempObj = it->second;
            int x = it->second.get_client_fd();
            std::pair<bool, int> newKey(prv, x);
            _users.insert(std::make_pair(newKey, tempObj));
            _users.erase(it);
            found = true;
            break; 
        }
    }
    if (!found) {
        throw "Nickname not found";
    }
    for (std::map<std::pair<bool, int>, cliente>::iterator it2 = _users.begin(); it2 != _users.end(); ++it2) {
        std::cout << "\033[0;31m" << "##################" << "\033[0m" << std::endl;
        std::cout << it2->second.get_nickname() << std::endl;
        std::cout << "\033[0;31m" << "##################" << "\033[0m" << std::endl;
    }
}