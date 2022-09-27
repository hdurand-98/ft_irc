/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:18:47 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 12:24:27 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	kick_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	user *newptr;
	std::string chan;
	std::string user;
	std::string comment;
	std::string rpl;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " KICK :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	if (count_arg(line) == 1)
		chan = line.substr(5, line.substr(5).find('\n'));
	else
		chan = line.substr(5, line.substr(5).find(' '));
	if (count_arg(line) == 2)
		user = line.substr(5 + chan.length() + 1, line.substr(5 + chan.length() + 1).find('\n'));
	else if (count_arg(line) > 2)
		user = line.substr(5 + chan.length() + 1, line.substr(5 + chan.length() + 1).find(' '));
	if (count_arg(line) > 2)
		comment = line.substr(5 + chan.length() + 1 + user.length() + 1, line.substr(5 + chan.length() + 1 + user.length() + 1).find('\n'));

	if (chan.length() == 0 || user.length() == 0)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " KICK :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	channel *index = getChan(data, chan);
	if (!index)
	{
		rpl = ":localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + ptr->getNick() + " " + chan + " :No such channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	if (!index->isMember(ptr))
	{
		rpl = ":localhost " + std::string(ERR_NOTONCHANNEL) + " " + ptr->getNick() + " " + chan + " :You're not on that channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	if (!index->isOp(ptr))
	{
		rpl = ":localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + ptr->getNick() + " " + chan + " :You're not channel operator\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	if (!index->isUserMember(ptr, user))
	{
		rpl = ":localhost " + std::string(ERR_USERNOTINCHANNEL) + " " + ptr->getNick() + " " + user + " " + chan + " :They aren't on that channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	newptr = index->getUserFromChan(ptr, user);
	if (comment.length() > 0)
	{
		if (comment[0] == ':' && comment.length() == 1)
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " KICK " + chan + " " + newptr->getNick() + " " + ptr->getNick() + "\r\n";
		else
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " KICK " + chan + " " + newptr->getNick() + " " + comment + "\r\n";
        send_to_all_members(rpl, index);
	}
    else
    {
		rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " KICK " + chan + " " + newptr->getNick() + " " + ptr->getNick() + "\r\n";
        send_to_all_members(rpl, index);
    } 

	index->deleteUserName(ptr, user);
}
