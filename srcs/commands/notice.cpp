/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:28:58 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 14:00:19 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	notice_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string nick;
	std::string msg;

	if (no_arg(line))
		return ;

	if (count_arg(line) == 1)
		nick = line.substr(7, line.substr(7).find('\n'));
	else
		nick = line.substr(7, line.substr(7).find(' '));
	
	if (count_arg(line) > 1)
		msg = line.substr(7 + nick.length() + 2, line.substr(7 + nick.length() + 2).find('\n'));

	user *index = NULL;
	std::vector<user *>::iterator it = data.users_vec.begin();
	while ( it != data.users_vec.end())
	{
		index = *it;
		if (index->getNick() == nick)
			break ;
		it++;
	}
	if (it == data.users_vec.end())
		return ;
	else
	{
		std::string rpl = ":" + ptr->getNick() + "!" + ptr->getUser() + "@" + ptr->getIp() + " NOTICE " + nick + " :" + msg + "\r\n";
		send(index->getSd(), rpl.c_str(), rpl.length(), 0);
	}
}
