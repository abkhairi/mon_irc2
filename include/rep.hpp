/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rep.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhairi <abkhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:02:22 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 16:31:09 by abkhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REP_HPP
#define REP_HPP


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define ERR_NICKNAMEINUSE(buffer)                                           "433 " + buffer + " " + buffer  + " :Nickname is already in use\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname)                             ":" + oldNick + " NICK " + nick + "\r\n"

#define RPL_ERRONEUSNICKNAME(nick, host)                                    ":" + host + " 432 " + nick + " :Erroneus nickname !\r\n"

#define RPL_JOIN(nick, username, channelname, ipaddress)                    ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define RPL_NAMREPLY(hostname, clients, channelname, nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname)                         ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define ERR_UNKNOWNMODE(host, nick, modechar)                                ":" + host + " 472 " + nick + " " + modechar + " :is unknown mode char to me\r\n"
#define RPL_NONICKNAMEGIVEN(nick, host)                                     ":" + host + " 431 " + nick + " :No nickname given !\r\n"
#define ERR_UNKNOWNCOMMAND(client, cmd)             	                    ":" + client + " 421 " + cmd + " :Unknown command\r\n"

#define NICKNAME_RPLY(nickname, username, hostname, newNickName)            ":" + nickname + "!~" + username + "@" + hostname + " NICK :" + newNickName  + "\r\n"
#define RPL_NOLIMITSET(nickname)                                            (": 663 " + nickname + ": No limit set\r\n")

#define ERR_BADCHANNELKEY(buffer, channel)  "475 " + buffer + " " + channel + " :Cannot join channel (+k)\r\n"

#define ERR_NONICKNAMEGIVEN(nick, hostname) ":" + hostname + " 431 " + nick + " :No nickname given\r\n"
#define RPL_PART(buffer,host,user, channel)                                 ":" + buffer + "!~" + user + "@" + host + " PART :" + channel + "\r\n" 

#define RPL_WELCOME(nick, hostname)  ":" + hostname + " 001 " + nick + " :Welcome " + nick + " to the ft_irc network !\r\n"
#define RPL_YOURHOST(nick, hostname) ":" + hostname + " 002 " + nick + " :Your host is " + hostname + " running version 1.0 !\r\n"
#define RPL_CREATED(nick, hostname, time_)  ":" + hostname + " 003 " + nick + " :This server was created " + time_ + " !\r\n"
#define RPL_MYINFO(nick, hostname)   ":" + hostname + " 004 " + nick + " :Host: " + hostname + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i, l !\r\n"
#define RPL_TOPIC(hostname, nick, channel, topic)                    ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, seter,username, setat)                      ":" + client + " 333 " + nick + " " + channel + " " + seter + "!~@abkhairi_irc :" + setat + "\r\n"
#define ERR_USERONCHANNEL(host,nick, channel)             ":" + host + " 443 " + nick + " " + channel + " :is already on channel\r\n"


#define RPL_ENDOFNAMES(hostname, nick, channelname)                         ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress)                    ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
#define ERR_NEEDMOREPARAM(client, cmd)             	                    ":" + client + " 461 " + cmd + " :Not enough parameters\r\n"

#define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n" 
#define ERR_ALREADYREGISTERED(nick, hostname) ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"

#define ERR_NOTREGISTERED(nick, hostname) ":" + hostname + " 451 " + nick + " :You have not registered !\r\n"

#define ERR_NOSUCHCHANNEL(hostname, channel, nick) ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"


#define RPL_INVITE(nick, username ,clienthostname, invited, channel)       ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"

#define ERR_NOTOP(hostname, channel) ":" + hostname + " 482 " + channel + " " + ":You're not a channel operator\r\n"
#define ERR_NOSUCHNICK(hostname, channel, argument) ":" + hostname + " 401 " + channel + " " + argument + " :No such nick/channel\r\n"

#define RPL_INVITING(hostname, inviting, invited, channel)                 ":" + hostname + " 341 " + inviting + " " + invited + " " + channel + " :Inviting " + invited + " to " + channel + "\r\n"

#define RPL_KICK(nick, username, host, kicked, channel, commntaire) ":" + nick + "!" + "~" + username + "@" + host + " KICK " + channel + " " + kicked + " " + commntaire + "\r\n"

#define RPL_NOTOPIC(hostname, nick, chann)   ":" + hostname + " 331 " + nick + " " + chann + " :No topic is set.\r\n"
#define RPL_TOPICDISPLAY(hostname, nick, channel, topic)                    ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(client, channel, nick, seter,username, setat)                      ":" + client + " 333 " + nick + " " + channel + " " + seter + "!~@abkhairi_irc :" + setat + "\r\n"
#define ERR_NOTONCHANNEL(client, channel)            "442 " + client + " " + channel + " :You're not on that channel\r\n"
#define ERR_CHANOPRIVSNEEDED(buffer, channel)                               ":" + buffer + " 482 " + channel + " :You're not channel operator\r\n"
#define RPL_NEWTOPICSETTED(nick, username, hostname, channel, topic)        ":" + nick + "!~" + username + "@" + hostname + " TOPIC " + channel + " " + topic + "\r\n"

#define ERR_CANNOTSENDTOCHAN(buffer, channel)                               "404 " + buffer + " " + channel + " :Cannot send to channel\r\n"
#define RPL_QUIT(nick, host, message)                                       ":" + nick + "!" + "~" + nick + "@" + host + " QUIT : " + message + "\r\n"

#define RPL_MODE(channel, nick, mode)                                      ":" + nick + " MODE " + channel + " " + mode + "\r\n"
#define RPL_NOTOP(nickname)                                                 (": 667 " + nickname + ": User not operator\r\n")
#define RPL_ALLOP(nickname)                                                 (": 666 " + nickname + ": User already operator\r\n")
#define RPL_ALLINV(nickname)                                                (": 665 " + nickname + ": Channel already set in invite-only mode\r\n")
#define RPL_NOTINV(nickname)                                                (": 664 " + nickname + ": Channel not in invite-only mode\r\n")
#define RPL_LIMITSET(nickname)                                              (": 662 " + nickname + ": This limit was set\r\n")
#define RPL_PASSNOTC(nickname)                                              (": 996 " + nickname + ": Password not correct\r\n")
#define RPL_NOPASSSET(nickname)                                             (": 997 " + nickname + ": No Password set\r\n")
#define RPL_NOKEY(nickname,channel, mode)                                   ": 661 " + nickname + " :" + channel + " " + mode + " * You must specify a parameter for the key mode. Syntax: <key>.\r\n"
#define RPL_ALREADYSET(nickname)                                            (": 998 " + nickname + ": this Password already set\r\n")
#define ERR_WASNOSUCHNICK(buffer,cmd)                                       ":" + buffer + " 406 " + cmd + " :There was no such nickname\r\n"

#endif