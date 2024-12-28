# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shamsate <shamsate@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/23 21:00:51 by shamsate          #+#    #+#              #
#    Updated: 2024/12/26 23:32:52 by shamsate         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME   = ircserv

CC     = c++

HEADER =  include/Client.hpp  include/rep.hpp  include/Server.hpp include/Channels.hpp

RM     = rm -rf

CFLAGS = -Wall -Wextra -Werror -std=c++98

FILES = main  src/Client  src/Server dependencies/UseFcnt src/HandleCmd  src/Authentification src/Channels\
		commands/join commands/kick commands/topic commands/privmsg commands/part commands/quit\
		commands/mode commands/invite\

SRC		= $(FILES:=.cpp)

OBJ		= $(FILES:=.opp)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
		@echo "\n* Compilation Starting $(NAME) ... ✔️ *\n"
		@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "* Execution ready. ✔️ *\n"

%.opp: %.cpp $(HEADER)
	@echo "* Making Object file $(notdir $@) from source file $(notdir $<) . ✔️ *\n"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@ $(RM) $(OBJ)
	@echo "\n* Removing object file .. ✔️ *\n"
	@echo "* Object file Removed. ✔️ *\n"

fclean: clean
	@ $(RM) $(NAME)
	@echo "* Removing $(NAME) .✔️ *\n"
	@echo "* Removed. ✔️ *\n"

re: fclean all

.PHONY: clean%