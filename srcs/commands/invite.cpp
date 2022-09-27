/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:22:19 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 11:01:32 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	invite_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string nickname;
	std::string chan;

	if (count_arg(line) < 2)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + "INVITE" + " :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	nickname = line.substr(7, line.substr(7).find(' '));
	chan = line.substr(7 + nickname.length() + 1, line.substr(7 + nickname.length() + 1).find('\n'));
	channel *index = getChan(data, chan);

	if (!index)
	{
		rpl = ":localhost " + std::string(ERR_NOTONCHANNEL) + " " + ptr->getNick() + " " + chan + " :You're not on that channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;	
	}
	if (!index->isMember(ptr))
	{
		rpl = ":localhost " + std::string(ERR_NOTONCHANNEL) + " " + ptr->getNick() + " " + chan + " :You're not on that channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;	
	}
	
	std::vector<user *>::iterator it = data.users_vec.begin();
	user *u;
	bool isExist = false;
	while (it != data.users_vec.end())
	{
		u = *it;
		if (u->getNick() == nickname)
		{
			isExist = true;
			break;
		}
		it++;
	}
	if (!isExist)
	{
		rpl = ":localhost " + std::string(ERR_NOSUCHNICK) + " " + ptr->getNick() + " " + nickname + " :No such nick/channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return;
	}
	if (index->isUserMember(ptr, nickname))
	{
		rpl = ":localhost " + std::string(ERR_USERONCHANNEL) + " " + ptr->getNick() + " " + nickname + " " + index->getName() + " :is already on channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return;
	}

	u->addInvitation(index->getName());

	rpl = ":" + ptr->getNick() + " INVITE " + nickname + " " + chan + "\r\n";
	send(u->getSd(), rpl.c_str(), rpl.length(), 0);

	rpl = ":server " + std::string(RPL_INVITING) + " NICK " + u->getNick() + " " + index->getName() + "\r\n";
	send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
}
