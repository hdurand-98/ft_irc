/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 14:32:33 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/20 14:58:11 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "libpack.hpp"

class user
{
    private:
        int                 _sd;
        struct sockaddr_in	_address;
        std::string         _buffer;
        bool                _accept; // for password
        std::string         _nick;
        std::string         _user;
        std::string         _real;
        bool                _nickbeforeuser;
        bool                _registred; // when user -> rpl
        std::vector<std::string>	_invited;

    public:
        user(){}
        user(int sd, sockaddr_in address) : _sd(sd), _address(address) {
            _accept = 0;
            _nickbeforeuser = 0;
            _registred = 0;
        }
    	~user(){}
        int			getSd() const {return (_sd);}
        sockaddr_in	getAddress() const {return (_address);}
        std::string getBuffer() const {return (_buffer);}
        void        setBuffer(std::string buffer) {_buffer = buffer;}
        bool        getAccept() const {return (_accept);}
        void        setAccept(bool accept) {_accept = accept;}
        void        cleanBuffer() {_buffer.clear();}
        std::string	getNick() const {return (_nick);}
        void		setNick(std::string nick){_nick = nick;}
        std::string	getUser() const {return (_user);}
        void		setUser(std::string user){_user = user;}
        std::string	getReal() const {return (_real);}
        void		setReal(std::string real){_real = real;}
        std::string	getIp() const {return (inet_ntoa(_address.sin_addr));}
        bool        getRegistred() const {return (_registred);}
        void        setRegistred(bool registred) {_registred = registred;}
        bool        getNickBeforeUser() const {return (_nickbeforeuser);}
        void        setNickBeforeUser(bool nickbeforeuser) {_nickbeforeuser = nickbeforeuser;}
        void		addInvitation(std::string name_chan) {_invited.push_back(name_chan);}

        bool		isInvited(std::string cli) const
        {
            for (std::vector<std::string>::const_iterator it = _invited.begin(); it != _invited.end(); it++)
            {
                std::string cursor = *it;
                if (cli == cursor)
                    return true;
            }
            return false;
        }
};

#endif