/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 11:30:55 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/22 15:36:25 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libpack.hpp"

void	disconnect(data &data)
{
	for(std::vector<user *>::iterator it = data.users_vec.begin(); it != data.users_vec.end(); ++it)
	{
		user *ptr = *it;

		
				for (std::vector<channel*>::iterator ite = data.channels_vec.begin(); ite != data.channels_vec.end(); ++ite)
					{
						if ((*ite)->isMember(ptr))
						{
							(*ite)->deleteUser(ptr);
							if ((*ite)->isOp(ptr))
								(*ite)->deleteOp(ptr);
							if ((*ite)->isMembersEmpty())
								delete_chan(data, *ite);
							--ite;
						}
					}
		delete ptr;
		data.users_vec.erase(it);
		it--;
	}
}

// function that returns an error message
bool	ret_error(std::string str)
{
	std::cerr << str << std::endl;
	return (1);
}

// function to parse port and password from args
bool parsing_arg(data &data, char **av)
{
    int	i = 0;

	while (av[1][i])
	{
		if (!isdigit(av[1][i]))
			return (ret_error("Port incorrect.\nPlease check your port again."));
		i++;
	}
	data.port = atoi(av[1]);
	if (av[2])
		data.password.assign(av[2], strlen(av[2]));
	else
		return (ret_error("Password cant be NULL.\nPlease try again witch another password."));
	return (0);
}


// compter le nombre d'arguments
int		count_arg(std::string str)
{
	int sp = 0;
	unsigned long i = 0;

	while (str[i])
	{
		if (str[i] == ' ')
			sp++;
		i++;
	}
	return (sp);
}

// check si il n'y a pas d'arguments apres la commande
bool	no_arg(std::string str)
{
	int sp = 0;
	unsigned long i = 0;

	while (str[i])
	{
		if (str[i] == ' ')
			sp++;
		i++;
	}
	if (sp > 0)
		return false;
	return true;
}

// delete un channel si il existe dans le vector channel
void	delete_chan(data &data, channel *chan)
{
	for (std::vector<channel *>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); it++)
	{
		if ((*it)->getName() == chan->getName())
		{
			delete *it;
			data.channels_vec.erase(it);
			return ;
		}
	}
}

// return un pointeur vers le channel si il existe
channel*	getChan(data &data, std::string name)
{
	for (std::vector<channel *>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); it++)
	{
		channel *index = *it;
		if (index->getName() == name)
			return index;
	}
	return NULL;
}

// send msg to every members in the channel but not to the sender
void	send_msg_to_all_members(std::string message, channel *channel, user *sender)
{
	user *c = NULL;
	std::vector<user*> members = channel->getMembers();
	for (std::vector<user*>::iterator it = members.begin(); it != members.end(); it++)
	{
		c = *it;
		if (sender->getNick() != c->getNick())
			send(c->getSd(), message.c_str(), message.length(), 0);
	}
}

// to list nicknames in a channel when a client join it
std::string	listClients(channel *chan_cmd)
{
	std::string liste;
	std::vector<user*> members = chan_cmd->getMembers();
	user *index;
	for (std::vector<user *>::iterator it = members.begin(); it != members.end(); it++)
	{
		index = *it;
		if (chan_cmd->isOp(index))
			liste += "@";
		liste += index->getNick() + " ";
	}
	return liste;
}

// send info about a client who join the chan to every member in the chan
void	send_to_all_members(std::string message, channel *channel)
{
	user *c = NULL;
	std::vector<user*> members = channel->getMembers();
	for (std::vector<user*>::iterator it = members.begin(); it != members.end(); it++)
	{
		c = *it;
		send(c->getSd(), message.c_str(), message.length(), 0);
	}
}

int		len(long nb)
{
	int		len;

	len = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

std::string		ft_itoa(int nb)
{
	std::string str;
	long	n;
	unsigned long		i;

	n = nb;
	i = len(n);
	str[i--] = '\0';
	if (n == 0)
	{
		str[0] = 48;
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n = n * -1;
	}
	while (n > 0)
	{
		str[i] = 48 + (n % 10);
		n = n / 10;
		i--;
	}
	return (str);
}


bool	isComa(std::string str)
{
	unsigned long i = 0;
	while (i < str.length())
	{
		if (str[i] == ',')
			return (1);
		i++;
	}
	return (0);
}