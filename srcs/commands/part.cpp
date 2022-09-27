/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 09:37:44 by codebrai          #+#    #+#             */
/*   Updated: 2022/06/24 14:19:20 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	part_cmd(data &data, user *ptr, std::string line)
{
    //line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    std::string chan;
    std::string comment;
    std::string rpl;

    if (no_arg(line))
    {
        rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + "PART" + " :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
    }

    if (count_arg(line) == 1)
		chan = line.substr(5, line.substr(5).find('\n'));
	else
		chan = line.substr(5, line.substr(5).find(' '));
    if (count_arg(line) > 1)
		comment = line.substr(5 + chan.length() + 1, line.substr(5 + chan.length() + 1).find('\n'));


    if (chan.length() == 0)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " PART :Not enough parameters\r\n";
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

    if (comment.length() > 0)
	{
        rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PART " + chan + " " + comment + "\r\n";
        send_msg_to_all_members(rpl, index, ptr);
        rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PART " + chan + " " + comment + "\r\n";
        send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
    else
    {
        rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PART " + chan + " " + ptr->getNick() + "\r\n";
        send_msg_to_all_members(rpl, index, ptr);
        rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " PART " + chan + " " + ptr->getNick() + "\r\n";
        send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
    } 
    
    if (index->isMember(ptr))
        index->deleteUser(ptr);
    if (index->isOp(ptr))
        index->deleteOp(ptr);
    if (index->isMembersEmpty())
        delete_chan(data, index);
}