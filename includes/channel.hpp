/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/27 11:27:55 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/20 17:59:26 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "libpack.hpp"

class channel
{
    private:
        std::string			_name;
        std::string         _key;
		std::string			_topic;
		std::vector<user*>	_members;
        std::vector<user*>	_operators;

    public:
        channel(std::string name) : _name(name) {}
		channel(std::string name, std::string key) : _name(name), _key(key) {}
		~channel() {_members.clear();}
        std::string			getName() const {return(_name);}
        std::string			getKey() const {return(_key);}
		std::string			getTopic() const {return(_topic);}
        void                setTopic(std::string topic) {_topic = topic;}
        std::vector<user*>	getMembers() {return(_members);}
        void                addUser(user *cli) {_members.push_back(cli);}
        void                addOp(user *cli) {_operators.push_back(cli);}

        bool		        isMember(user *cli) const
        {
            for (std::vector<user*>::const_iterator it = _members.begin(); it != _members.end(); it++)
            {
                user *c = *it;
                if (c->getNick() == cli->getNick())
                    return true;
            }
            return false;
        }

        bool                isOp(user *cli) const
        {
            for (std::vector<user*>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
            {
                user *c = *it;
                if (c->getNick() == cli->getNick())
                    return true;
            }
            return false; 
        }

        void		        deleteUser(user *cli)
        {
            for (std::vector<user *>::iterator it = _members.begin(); it != _members.end(); it++)
            {
                if (*it == cli)
                {
                    _members.erase(it);
                    return ;
                }
            }
        }

        void		        deleteOp(user *cli)
        {
            for (std::vector<user *>::iterator it = _operators.begin(); it != _operators.end(); it++)
            {
                if (*it == cli)
                {
                    _operators.erase(it);
                    return ;
                }
            }
        }

        bool                isUserMember(user *cli, std::string username)
        {
            for (std::vector<user*>::const_iterator it = _members.begin(); it != _members.end(); it++)
            {
                user *c = *it;
                if (c->getNick() == username)
                    return true;
            }
            return false; 
            (void)cli;
        }

        user*                getUserFromChan(user *cli, std::string username)
        {
            for (std::vector<user*>::const_iterator it = _members.begin(); it != _members.end(); it++)
            {
                user *c = *it;
                if (c->getNick() == username)
                    return c;
            }
            return NULL; 
            (void)cli;
        }

        void		        deleteUserName(user *cli, std::string username)
        {
            for (std::vector<user *>::iterator it = _members.begin(); it != _members.end(); it++)
            {
                user *c = *it;
                if (c->getNick() == username)
                {
                    _members.erase(it);
                    return ;
                }
            }
            (void)cli;
        }

        bool	isMembersEmpty()
        {
            if (_members.size() == 0)
                return true;
            return false;
        }
};

#endif