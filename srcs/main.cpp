/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 19:03:16 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/21 11:37:36 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libpack.hpp"

#include <stdio.h>

int loop = 1;

void sig_handler(int signo)
{
	loop = 0;
	printf(" -> signal catched : [%d] -> server is closed.\n", signo);
}

int main(int ac, char **av)
{
    if (ac != 3)
        return (ret_error("Number of arguments incorrect.\nPlease follow the rules ./ircserv <port> <password>."));
	

    data data;

	signal(SIGINT, sig_handler);

	if (parsing_arg(data, av))
		return (1);

	if (socket_creation(data))
		return (1);

    return (0);
}
