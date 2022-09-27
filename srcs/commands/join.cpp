/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:36:02 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 14:13:43 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

bool	chan_is_valid(std::string chan)
{
	if (chan[0] != '&' && chan[0] != '#' && chan[0]!= '+' && chan[0] != '!')
		return(0);
	if (chan.length() > 50)
		return(0);
	int i = 0;
	while (chan[i])
	{
		if (chan[i] == ' ' || chan[i] == 7 || chan[i] == ',')
			return(0);
		i++;
	}
	return (1);
}

void	join_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string chan;
	std::string key;
	std::string rpl;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " JOIN" + " : Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	// take chan or chan + key
	if (count_arg(line) == 1)
		chan = line.substr(5, line.substr(5).find('\n'));
	else
	{
		chan = line.substr(5, line.substr(5).find(' '));
		key = line.substr(5 + chan.length() + 1, line.substr(5 + chan.length() + 1).find('\n'));
	}

	// if no chan -> NEEDMOREPARAMS
	if (chan.length() == 0 || chan.length() == 1)
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " JOIN" + " : Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	// check if the chan already exist in channels_vec
	channel *index = NULL;
	std::vector<channel *>::iterator it = data.channels_vec.begin();
	while ( it != data.channels_vec.end())
	{
		index = *it;
		if (index->getName() == chan)
			break ;
		it++;
	}
	// if the chan dont exist
	if (it == data.channels_vec.end())
	{
		// check is the chan is valid
		if (!chan_is_valid(chan))
		{
			rpl = ":localhost " + std::string(ERR_NOSUCHCHANNEL) + " " + ptr->getNick() + " " + chan +  " :No such channel\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}
		// if the chan is valid -> create it and join it as operator
		else
		{
			if (key.length() > 0)
				data.channels_vec.push_back(new channel(chan, key));
			else
				data.channels_vec.push_back(new channel(chan));
			// add in user and ope
			index = getChan(data, chan);
			index->addUser(ptr);
			index->addOp(ptr);
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " JOIN " + chan + "\r\n";
			send_to_all_members(rpl, index);
			// display topic to the client
			rpl = ":localhost " + std::string(RPL_TOPIC) + " " + ptr->getNick() + " " + chan + " :" + index->getTopic() + "\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			// display members in the chan
			rpl = ":localhost " + std::string(RPL_NAMREPLY) + " " + ptr->getNick() + " = " + index->getName() + " : " + listClients(index) + "\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			rpl = ":localhost " + std::string(RPL_ENDOFNAMES) + " " + ptr->getNick() + " " + index->getName() + " : End of NAMES list\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
	}
	// if the chan exist -> join it
	else
	{
		// check password before
		if (key != index->getKey() && !ptr->isInvited(index->getName()))
		{
			rpl = ":localhost " + std::string(ERR_BADCHANNELKEY) + " " + ptr->getNick() + " " + chan +  " :Cannot join channel (key)\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}
		// add the client in _members vec
		index->addUser(ptr);
		rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " JOIN " + chan + "\r\n";
		send_to_all_members(rpl, index);
		// display topic to the client
		rpl = ":localhost " + std::string(RPL_TOPIC) + " " + ptr->getNick() + " " + chan + " :" + index->getTopic() + "\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		// display members in the chan
		rpl = ":localhost " + std::string(RPL_NAMREPLY) + " " + ptr->getNick() + " = " + index->getName() + " : " + listClients(index) + "\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		rpl = ":localhost " + std::string(RPL_ENDOFNAMES) + " " + ptr->getNick() + " " + index->getName() + " : End of NAMES list\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
}
