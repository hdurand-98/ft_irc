/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:31:23 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 13:54:41 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	privmsg_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string nick;
	std::string msg;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " PRIVMSG :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	if (count_arg(line) == 1)
		nick = line.substr(8, line.substr(8).find('\n'));
	else
		nick = line.substr(8, line.substr(8).find(' '));
	if (count_arg(line) > 1)
		msg = line.substr(8 + nick.length() + 2, line.substr(8 + nick.length() + 2).find('\n'));

	user *index = NULL;
	if (nick.length() < 2)
	{
		rpl = ":localhost " + std::string(ERR_NORECIPIENT) + " " + ptr->getNick() + " :No recipient given (PRIVMSG)\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	if (msg.length() == 0)
	{
		rpl = ":localhost " + std::string(ERR_NOTEXTTOSEND) + " " + ptr->getNick() + " :No text to send\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	std::vector<user *>::iterator it = data.users_vec.begin();
	while ( it != data.users_vec.end())
	{
		index = *it;
		if (index->getNick() == nick)
			break ;
		it++;
	}
	if (nick[0] == '#' || nick[0] == '&' || nick[0] == '+' || nick[0] == '!')
	{
		channel *chan = getChan(data, nick);
		if (chan == NULL)
		{
			rpl = ":localhost " + std::string(ERR_CANNOTSENDTOCHAN) + " " + ptr->getNick() + " " + nick + " :Cannot send to channel\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}
		if (!chan->isMember(ptr))
		{
			rpl = ":localhost " + std::string(ERR_CANNOTSENDTOCHAN) + " " + ptr->getNick() + " " + nick + " :Cannot send to channel\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}
		rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PRIVMSG " + nick + " :" + msg + "\r\n";
		send_msg_to_all_members(rpl, chan, ptr);
	}
	else if (it == data.users_vec.end())
	{
		rpl = ":localhost " + std::string(ERR_NOSUCHNICK) + " " + ptr->getNick() + " " + nick +  " :No such nick/channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	else
	{
		rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PRIVMSG " + nick + " :" + msg + "\r\n";
		send(index->getSd(), rpl.c_str(), rpl.length(), 0);
	}
}
