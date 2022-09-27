/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/12 10:58:02 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 11:11:38 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

#include "libpack.hpp"
#include "user.hpp"
#include "channel.hpp"

class data
{
    public:
        int                 port;
        std::string         password;
        int                 master_socket;
        struct sockaddr_in  address;
        fd_set              readfds;
        std::vector<user *>	users_vec;
        std::vector<channel *> channels_vec;


    data(){}
	~data(){}
};

#endif