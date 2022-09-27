/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 16:22:17 by codebrai          #+#    #+#             */
/*   Updated: 2022/06/24 16:36:41 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libpack.hpp"

std::vector<std::string> ft_join_parsing(std::string line)
{
	std::vector<std::string> res;
	std::string keys;
	std::string channels = line.substr(line.find(' ') + 1, line.rfind(' ') - (line.find(' ') + 1));
	if (line.rfind(' ') != line.find(' '))
		keys = line.substr(line.rfind(' ') + 1);

	size_t word_start = 0;
	if (channels.find(',', word_start) == std::string::npos)
	{
		if (keys.empty() == true)
			res.push_back("JOIN " + channels);
		else
			res.push_back("JOIN " + channels + " " + keys);
		return (res);
	}

	//ajoute tous les channels dans res
	while (word_start != std::string::npos)
	{
		res.push_back("JOIN " + channels.substr(word_start, channels.find(',', word_start) - word_start));
		if (channels.find(',', word_start) == std::string::npos)
			break;
		else
			word_start = channels.find(',', word_start) + 1;
	}

	word_start = 0;
	size_t i = 0;
	while (word_start != std::string::npos && keys.empty() == false && i < res.size())
	{
		std::string tmp_key = keys.substr(word_start, keys.find(',', word_start) - word_start);
		if (tmp_key != "x")
			res[i].append(" " + tmp_key);
		if (keys.find(',', word_start) == std::string::npos)
			break;
		else
			word_start = keys.find(',', word_start) + 1;
		i++;
	}

	return (res);
}

std::vector<std::string> ft_part_parsing(std::string line)
{
	std::vector<std::string> res;
	std::string current;
	std::string channels;
	std::string comment;
	size_t word_start = 0;

	word_start = line.find(' ') + 1;
	while (word_start != std::string::npos)
	{
		if (line.find(',', word_start) != std::string::npos)
		{
			res.push_back("PART " + line.substr(word_start, line.find(',', word_start) - word_start));
			word_start = line.find(',', word_start) + 1;
		}
		else
		{
			res.push_back("PART " + line.substr(word_start, line.find(' ', word_start) - word_start));
			break;
		}
	}

	word_start = line.find(':');
	if (word_start != std::string::npos)
	{
		line = line.substr(line.find(':') + 1);
		comment = line.substr(0, line.find(' '));
	}

	std::vector<std::string>::iterator it = res.begin();
	while (it != res.end())
	{
		if (comment.empty() == false)
			it->append(" :" + comment);
		it++;
	}

	return (res);
}

std::vector<std::string> ft_kick_parsing(std::string line)
{
	std::vector<std::string> res;
	std::vector<std::string> users_str;
	std::string channels;
	std::string users;
	std::string comment;

	line = line.substr(line.find(' ') + 1);
	channels = line.substr(0, line.find(' '));
	users = line.substr(line.find(' ') + 1, line.rfind(' ') - line.find(' ') - 1);
	comment = line.substr(line.find(':'));

	size_t word_start = 0;
	while (word_start != std::string::npos)
	{
		users_str.push_back(users.substr(word_start, users.find(',', word_start) - word_start));
		if (users.find(',', word_start) == std::string::npos)
			break;
		else
			word_start = users.find(',', word_start) + 1;
	}

	word_start = 0;
	while (word_start != std::string::npos)
	{
		for (size_t i = 0; i < users_str.size(); i++)
			res.push_back("KICK " + channels.substr(word_start, channels.find(',', word_start) - word_start));
		if (channels.find(',', word_start) == std::string::npos)
			break;
		else
			word_start = channels.find(',', word_start) + 1;
	}

	std::vector<std::string>::iterator it2 = users_str.begin();
	for (std::vector<std::string>::iterator it = res.begin(); it != res.end(); it++, it2++)
	{
		if (it2 == users_str.end())
			it2 = users_str.begin();
		it->append(" " + *it2 + " " + comment); 
	}

	return (res);
}

void	cmd_processing(data &data, user *ptr, std::string buffer)
{
	std::stringstream ss(buffer);
	std::string line;
	std::string rpl;

	while (std::getline(ss, line, '\n'))
	{
		if (*line.rbegin() ==  '\r')
			line = line.substr(0, line.find("\r"));
		std::string cmd = line.substr(0, line.find(' '));

		if (ptr->getAccept() == 0)
			pass_cmd(data, ptr, line, cmd);
		else if (cmd == "NICK")
			nick_cmd(data, ptr, line);
		else if (cmd == "USER")
			user_cmd(data, ptr, line);
		else if (ptr->getRegistred() == 0)
		{
			rpl = ":server " + std::string(ERR_NOTREGISTERED) + " :You have not registered\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
		else if (cmd == "NOTICE")
			notice_cmd(data, ptr, line);
		else if (cmd == "PRIVMSG")
			privmsg_cmd(data, ptr, line);
		else if (cmd == "JOIN")
		{
			std::vector<std::string> join_cmds = ft_join_parsing(line);
			for (std::vector<std::string>::iterator it = join_cmds.begin(); it != join_cmds.end(); it++)
				join_cmd(data, ptr, *it);
		}
		else if(cmd == "PASS")
		{
			rpl = ":server " + std::string(ERR_ALREADYREGISTRED) + " " + cmd + " : Unauthorized command (already registered)\r\n";
			send(ptr->getSd(), rpl.c_str(), rpl.length(), 0);
		}
		else if (cmd == "TOPIC")
			topic_cmd(data, ptr, line);
		else if (cmd == "KICK")
		{
			std::vector<std::string> kick_cmds = ft_kick_parsing(line);
			for (std::vector<std::string>::iterator it = kick_cmds.begin(); it != kick_cmds.end(); it++)
				kick_cmd(data, ptr, *it);
		}
		else if (cmd == "LIST")
			list_cmd(data, ptr, line);
		else if (cmd == "NAMES")
			names_cmd(data, ptr, line);
		else if (cmd == "INVITE")
			invite_cmd(data, ptr, line);
		else if (cmd == "PART")
		{
			std::vector<std::string> part_cmds = ft_part_parsing(line);
			for (std::vector<std::string>::iterator it = part_cmds.begin(); it != part_cmds.end(); it++)
				part_cmd(data, ptr, *it);
		}
		else if (cmd == "PING")
			ping_cmd(data, ptr, line);
		else if (cmd == "QUIT")
			quit_cmd(data, ptr, line);
	}
}
