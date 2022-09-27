/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:21:14 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/23 10:12:14 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	names_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string chan;
	char c = '=';

	if (count_arg(line) == 1)
		chan = line.substr(6, line.substr(6).find('\n'));
	else if (count_arg(line) > 1)
		chan = line.substr(6, line.substr(6).find(' '));

	if (chan.length())
	{
		unsigned long i = 0;
		std::string newchan;
		channel *index;
		if (isComa(line))
		{
			while (i < chan.length())
			{
				if (chan[i] != ',')
					newchan.push_back(chan[i]);
				if (chan[i] == ',')
				{
					index = getChan(data, newchan);
					if (index)
					{
						if (index->getKey().length())
							c = '*';
						rpl = ":" + std::string(RPL_NAMREPLY) + " " + c + index->getName() + " : " + listClients(index) + "\r\n";
						send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
					}	
					newchan.clear();
				}
				i++;
			}
			index = getChan(data, newchan);
			if (index)
			{
				if (index->getKey().length())
					c = '*';
				rpl = ":" + std::string(RPL_NAMREPLY) + " " + c + index->getName() + " : " + listClients(index) + "\r\n";
				send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			}	
			newchan.clear();
			
		}
		else
		{
			index = getChan(data, chan);
			if (index)
			{
				if (index->getKey().length())
					c = '*';
				rpl = ":" + std::string(RPL_NAMREPLY) + " " + c + index->getName() + " : " + listClients(index) + "\r\n";
				send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			}
		}
		rpl = ":" + std::string(RPL_ENDOFNAMES) + " :End of NAMES list\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
	else
	{
		for (std::vector<channel *>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); it++)
		{
			c = '=';
			if ((*it)->getKey().length())
				c = '*';
			rpl = ":" + std::string(RPL_NAMREPLY) + " " + c + (*it)->getName() + " : " + listClients(*it) + "\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
		int onchan = 0;
		std::string usernotonchan;
		for (std::vector<user *>::iterator it = data.users_vec.begin(); it != data.users_vec.end(); it++)
		{
			onchan = 0;
			for (std::vector<channel *>::iterator ite = data.channels_vec.begin(); ite != data.channels_vec.end(); ite++)
			{
				if ((*ite)->isMember(*it))
					onchan++;
			}
			if (onchan == 0)
				usernotonchan = usernotonchan + " " + (*it)->getNick();
		}

		rpl = ":" + std::string(RPL_NAMREPLY) + " " + '*' + " :" + usernotonchan + "\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);

		rpl = ":" + std::string(RPL_ENDOFNAMES) + " :End of NAMES list\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}

}
