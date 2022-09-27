/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:17:23 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 14:22:51 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	topic_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string chan;
	std::string topic;
	std::string rpl;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + "TOPIC" + " :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;	
	}

	if (count_arg(line) == 1)
		chan = line.substr(6, line.substr(6).find('\n'));
	else
		chan = line.substr(6, line.substr(6).find(' '));
	if (count_arg(line) > 1)
		topic = line.substr(6 + chan.length() + 1, line.substr(6 + chan.length() + 1).find('\n'));

	if (chan.length() == 0)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + "TOPIC" + " :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	channel *index = getChan(data, chan);
	if (!index || !index->isMember(ptr))
	{
		rpl = ":localhost " + std::string(ERR_NOTONCHANNEL) + " " + ptr->getNick() + " " + chan + " :You're not on that channel\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	// check topic
	if (topic.length() == 0)
	{
		if (index->getTopic().length() > 0)
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " TOPIC " + index->getName() + " :" + index->getTopic() + "\r\n";
		else
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " TOPIC " + index->getName() + " :" + index->getTopic() + "\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
	// change topic
	else if (topic.length() > 0)
	{
		if (index->isOp(ptr))
		{
			if (topic[0] == ':')
				topic.erase(0, 1);
			index->setTopic(topic);
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " TOPIC " + index->getName() + " :" + index->getTopic() + "\r\n";
			send_to_all_members(rpl, index);
		}
		else
		{
			rpl = ":localhost " + std::string(ERR_CHANOPRIVSNEEDED) + " " + ptr->getNick() + " " + index->getName() + " :You're not channel operator\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
	}
}
