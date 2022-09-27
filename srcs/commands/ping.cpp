/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/17 16:23:37 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 14:19:59 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	ping_cmd(data &data, user *ptr, std::string line)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string response;
	std::string rpl;

	if (no_arg(line))
	{
		rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + "PING" + " :Not enough parameters\r\n";
		send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}

	response = line.substr(5, line.substr(5).find('\n'));

	rpl = ":server PONG " + response + "\r\n";
	send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);

	(void)data;
}
