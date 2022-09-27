/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:25:11 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 16:32:20 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	user_cmd(data &data, user *ptr, std::string line)
{
	(void)data;
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;

	if (ptr->getNickBeforeUser() == 0)
		return ;

	if (ptr->getRegistred() == 1)
	{
		rpl = ":localhost " + std::string(ERR_ALREADYREGISTRED) + " " + ptr->getNick() + " :Unauthorized command (already registered)\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	int i = 0;
	int sp = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			sp++;
		i++;
	}
	if (sp < 4)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " USER" + " : Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	ptr->setUser(line.substr(5, line.substr(5).find(' ')));

	i = 0;
	sp = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			sp++;
		if (sp == 4)
			break ;
		i++;
	}
	ptr->setReal(line.substr(i + 2, line.find('\n')));

	if (ptr->getRegistred() == 0)
	{
		ptr->setRegistred(1);

		rpl = ":localhost 001 " + ptr->getNick() + " :Welcome to the Internet Relay Network " + ptr->getNick() + "!" + ptr->getUser() + "@" + "localhost\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		rpl = ":localhost 002 " + ptr->getNick() + " :Your host is localhost, running version 1.0\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		rpl = ":localhost 003 " + ptr->getNick() + " :This server was created 12/06/2022\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		rpl = ":localhost 004 " + ptr->getNick() + " :localhost 1.0\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		rpl = ":localhost 005 " + ptr->getNick() + " :Try server localhost, port " + ft_itoa(data.port).c_str() + "\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
}
