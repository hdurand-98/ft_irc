/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:19:58 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/22 15:36:53 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	list_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string chan;

	if (count_arg(line) == 1)
		chan = line.substr(5, line.substr(5).find('\n'));
	else if (count_arg(line) > 1)
		chan = line.substr(5, line.substr(5).find(' '));

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
						rpl = ":" + std::string(RPL_LIST) + " " + index->getName() + " : " + index->getTopic() + "\r\n";
						send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
					}	
					newchan.clear();
				}
				i++;
			}
			index = getChan(data, newchan);
			if (index)
			{
				rpl = ":" + std::string(RPL_LIST) + " " + index->getName() + " : " + index->getTopic() + "\r\n";
				send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			}	
			newchan.clear();
			
		}
		else
		{
			index = getChan(data, chan);
			if (index)
			{
				rpl = ":" + std::string(RPL_LIST) + " " + index->getName() + " : " + index->getTopic() + "\r\n";
				send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			}
		}
		rpl = ":" + std::string(RPL_LISTEND) + " :End of LIST\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
	else
	{
		for (std::vector<channel *>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); it++)
		{
			rpl = ":" + std::string(RPL_LIST) + " " + (*it)->getName() + " : " + (*it)->getTopic() + "\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
		rpl = ":" + std::string(RPL_LISTEND) + " :End of LIST\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
	}
}
