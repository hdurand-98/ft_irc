/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:22:13 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 16:16:24 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

bool	is_nick_syntax_ok(std::string nick)
{
	if (nick.length() > 9)
		return (0);
	if ( !(nick[0] <= 'Z' && nick[0] >= 'A') && !(nick[0] <= 'z' && nick[0] >= 'a')
		&& nick[0] != '[' && nick[0] != ']' && nick[0] != '\\' && nick[0] != '`' && nick[0] != '_'
		&& nick[0] != '^' && nick[0] != '{' && nick[0] != '}' && nick[0] != '|' )
		return (0);
	int i = 1;
	while(nick[i])
	{
		if ( !(nick[i] <= 'Z' && nick[i] >= 'A') && !(nick[i] <= 'z' && nick[i] >= 'a')
		&& nick[i] != '[' && nick[i] != ']' && nick[i] != '\\' && nick[i] != '`' && nick[i] != '_'
		&& nick[i] != '^' && nick[i] != '{' && nick[i] != '}' && nick[i] != '|'
		&& nick[i] != '-'
		&& !(nick[i] <= '9' && nick[i] >= '0'))
			return (0);
		i++;
	}
	return (1);
}

user*	is_nick_already_taken(std::string name, std::vector<user *> users)
{
	user *index = NULL;
	for (std::vector<user*>::iterator it = users.begin(); it != users.end(); it++)
	{
		index = *it;
		if (index->getNick() == name)
			return index;
	}
	return NULL;
}

void	nick_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string nick;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NONICKNAMEGIVEN) + " :No nickname given\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	if (count_arg(line) == 1)
		nick = line.substr(5, line.find('\n'));
	else
		nick = line.substr(5, line.find(' '));

	if (is_nick_syntax_ok(nick) == 0)
	{
		rpl = ":localhost " + std::string(ERR_ERRONEUSNICKNAME) + " " + nick + " :Erroneous nickname\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	if (is_nick_already_taken(nick, data.users_vec) && ptr->getRegistred() == 0)
	{
		rpl = ":localhost " + std::string(ERR_NICKNAMEINUSE) + " " + nick + " " + nick + " :Nickname is already in use\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
	if (is_nick_already_taken(nick, data.users_vec) && ptr->getRegistred() == 1)
	{
		rpl = ":localhost " + std::string(ERR_NICKNAMEINUSE) + " " + ptr->getNick() + " " + nick + " :Nickname is already in use\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " NICK " + nick + "\r\n";
	send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	ptr->setNick(nick);
	ptr->setNickBeforeUser(1);
}
