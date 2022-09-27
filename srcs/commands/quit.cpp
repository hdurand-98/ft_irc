/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:14:53 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 11:11:19 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"


void	quit_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string message;

	if (count_arg(line) > 0)
		message = line.substr(5, line.substr(5).find('\n'));

	// std::string quitmsg = line.substr(5, line.substr(5).find('\n'));

	for (std::vector<channel*>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); it++)
	{
		if ((*it)->isMember(ptr))
		{
			rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " QUIT " + message + "\r\n";
			send_msg_to_all_members(rpl, *it, ptr);
		}
	}

	rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " QUIT " + message + "\r\n";
	send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);

	(void)data;
}
