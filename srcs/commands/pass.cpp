/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 20:19:29 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 16:42:06 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libpack.hpp"

void	pass_cmd(data &data, user *ptr, std::string line, std::string cmd)
{
	//line.erase(remove(line.begin(), line.end(), '\r'), line.end());
	std::string rpl;
	std::string pass;

	if (no_arg(line))
	{
		// rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " " + cmd + " :Not enough parameters\r\n";
		// send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
	
	if (cmd == "PASS")
	{

		pass = line.substr(5, line.find('\n'));

		if (pass.length() == 0)
		{
			rpl = ":localhost " + std::string(ERR_NEEDMOREPARAMS) + " PASS :Not enough parameters\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}

		if (data.password == pass)
			ptr->setAccept(1);
		else
		{
			// rpl = ":server " + std::string(ERR_PASSWDMISMATCH) + " : Password incorrect\r\n";
			// send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
			return ;
		}
	}
	else if (cmd == "CAP")
		return ;
	else
	{
		// rpl = ":server " + std::string(ERR_NEEDMOREPARAMS) + " " + cmd + " :Not enough parameters (please provide password first)\r\n";
		// send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		return ;
	}
}
