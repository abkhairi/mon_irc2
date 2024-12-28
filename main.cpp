/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhairi <abkhairi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:01:05 by shamsate          #+#    #+#             */
/*   Updated: 2024/12/26 16:19:48 by abkhairi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"
//::::::::::::::::::::::::::::The IRC Main():::::::::::::::::::::::::::::::::::
int main(int ac, char** av){
    if (ac != 3){
        std::cout << "Invalid args :( " << std::endl;
        return  1;
    }
    std::string port = av[1];
    std::string pass = av[2];

    int port_int = parsingPortPass(port, pass);
    if (port_int == 1)
    {
        std::cout << "Error in Port :(" << std::endl;
        return 1;
    }
    if (port_int == 2){
        std::cout << "Error in Password :(" << std::endl;
        return 1;}
    Server irc(port_int, pass);
    size_t &i = irc.cliIdx;
    irc.init_serv(port_int, pass, i);
    return 0;
};
//::::::::::::::::::::::::::::The End():::::::::::::::::::::::::::::::::::