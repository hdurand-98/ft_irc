# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/11 19:01:15 by corentindeb       #+#    #+#              #
#    Updated: 2022/06/21 15:52:33 by codebrai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98

SRCS =      ./srcs/main.cpp \
			./srcs/socket.cpp \
			./srcs/commands.cpp \
			./srcs/utils.cpp \
			./srcs/commands/pass.cpp \
			./srcs/commands/nick.cpp \
			./srcs/commands/user.cpp \
			./srcs/commands/notice.cpp \
			./srcs/commands/privmsg.cpp \
			./srcs/commands/join.cpp \
			./srcs/commands/quit.cpp \
			./srcs/commands/topic.cpp \
			./srcs/commands/kick.cpp \
			./srcs/commands/list.cpp \
			./srcs/commands/names.cpp \
			./srcs/commands/invite.cpp \
			./srcs/commands/ping.cpp \
			./srcs/commands/part.cpp

OBJSRCS = $(SRCS:.cpp=.o)

all: ${NAME}

$(NAME) : $(OBJSRCS)
		@c++ -o $(NAME) $(FLAGS) $(OBJSRCS)

%.o: %.cpp
		c++ $(FLAGS) -o $@ -c $<

clean:
		${RM} ${OBJSRCS}

fclean: clean
		${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re