/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codebrai <codebrai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 19:34:50 by corentindeb       #+#    #+#             */
/*   Updated: 2022/06/24 11:11:32 by codebrai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libpack.hpp"

bool socket_creation(data &data)
{
	int opt = 1;
	int	max_sd, sd, activity, new_socket, addrlen, valread = 0;
	char	buffer[1024] = {0};


	// creer l'objet socket dans un master socket
	// AF_INET = IPV4 PROTOCOL
	// SOCK_STREAM = TCP
	// valeur du champ protocol generalement 0
    if ((data.master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		return(ret_error("socket failed"));

	// configurer le descripteur de socket que l'on vient de creer
	// le fichier de descripteur de socket
	// SOL_SOCKET = niveau d'application de l'option
	// SO_REUSEADDR = nom de l'option -> permet de reutiliser l'adresse
	// de la socket tout de suite
	// opt = utiliser pour acceder aux options de la socket
	if (setsockopt(data.master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		return(ret_error("setsockopt failed"));
	
	// structure symbolisant l'adresse
	// AF_INET = IPV4 PROTOCOL
	// The htons() function converts the unsigned short integer hostshort
	// from host byte order to network byte order. 
	data.address.sin_family = AF_INET;
	data.address.sin_addr.s_addr = INADDR_ANY;
	data.address.sin_port = htons(data.port);

	// bind pour attacher la socket a un port internet et une adresse ip
	// le fichier descripteur de socket
	// addr -> une structure symbolisant l'adresse
	if (bind(data.master_socket, (struct sockaddr *)&data.address, sizeof(data.address)) < 0)
		return ret_error("bind failed");
	
	// On peut maintenant passer la socket en etat d'ecoute
	// Elle est prete a recevoir des connexions
	// le fichier descripteur de la socket
	// 3 = taille de la file d'attente de la socket
	if (listen(data.master_socket, 3) < 0)
		return ret_error("listen failed");

	// fcntl pour modifier un descripteur de fichier
	// le fichier descripteur de la socket
	// la commande de modification 
	// argument correspondant a la commande
	// On va modifier le drapeau F_SETFL de la socket pour le rendre non bloquant O_NONBLOCK
	// Si le descripteur reste bloquant, la fonction accept ne rendra pas la main
	// entrainant le blocage complet du programme
	if (fcntl(data.master_socket, F_SETFL, O_NONBLOCK) == -1)
		return ret_error("fcntl failed");
	
	addrlen = sizeof(&data.address);

	std::cout << BCYN << "[ircserv starts] -> waiting for connections..." << RESET_CLR << std::endl;

	while (loop)
	{
		// Macro de select()
		// permet d'effacer un ensemble
		FD_ZERO(&data.readfds);

		// Macro de select()
		// permet d'ajouter un descripteur de fichier dans un ensemble
		FD_SET(data.master_socket, &data.readfds);
		max_sd = data.master_socket;

		// add child sockets to set
		for(std::vector<user *>::iterator it = data.users_vec.begin(); it != data.users_vec.end(); it++)
		{
			user *ptr = *it;

			// socket descriptor
			sd = ptr->getSd();

			// if valid socket descriptor then add to read list
            if (sd > 0)
            	FD_SET(sd, &data.readfds);
			
			// highest file descriptor number, need it for the select function
            if (sd > max_sd)
            	max_sd = sd;
		}

		// permet de surveiller plusieurs descripteurs de fichier
        activity = select(max_sd + 1, &data.readfds, NULL, NULL, NULL);

		if ((activity < 0) && (errno!=EINTR))  
			std::cerr << "select error" << std::endl;
		
		// if something happened on the master socket, then its an incoming connection
		// Macro de select()
		// verifie si un descripteur de fichier est dans un ensemble
		if (FD_ISSET(data.master_socket, &data.readfds))
		{
			// Avec accept, on peut maintenant attendre une connexion
			// le fichier descripteur de la socket
			// l'adresse du client
			// la taille de la structure addr
			if ((new_socket = accept(data.master_socket, (struct sockaddr *)&data.address, (socklen_t*)&addrlen)) < 0)
			{
				disconnect(data);
				break ;
				ret_error("accept failed");
			}
			
			// print the new incoming connection
			// std::cout << BBLU << "[New connection] -> socket fd : " << new_socket << " | ip : " << inet_ntoa(data.address.sin_addr) << " | port : " << ntohs(data.address.sin_port) << RESET_CLR << std::endl;
			std::cout << BBLU << "[Client connected] -> sd : " << new_socket << " | ip : " << inet_ntoa(data.address.sin_addr) << RESET_CLR << std::endl;

			// add new socket to vector of sockets
			data.users_vec.push_back(new user(new_socket, data.address));
		}

		// input/output operation on some other socket
		for(std::vector<user *>::iterator it = data.users_vec.begin(); it != data.users_vec.end(); it++)
		{
			user *ptr = *it;
			sd = ptr->getSd();
			if (FD_ISSET(sd, &data.readfds))
			{
				// check if it is a deconnexion
				// On utilise read pour lire dans une socket
				if ((valread = read(sd, buffer, 1024)) <= 0)
				{
					// somebody disconnected, get his details and print 
					// std::cout << BBLU << "[Client disconnected] -> socket fd : " << new_socket << " | ip : " << inet_ntoa(data.address.sin_addr) << " | port : " << ntohs(data.address.sin_port) << RESET_CLR << std::endl;
					std::cout << BBLU << "[Client disconnected] -> sd : " << sd << " | ip : " << inet_ntoa(data.address.sin_addr) << RESET_CLR << std::endl;
					// close the socket and mark as 0 in list for reuse
					close(sd);
					// delete user from every channel
					for (std::vector<channel*>::iterator it = data.channels_vec.begin(); it != data.channels_vec.end(); ++it)
					{
						if ((*it)->isMember(ptr))
						{
							(*it)->deleteUser(ptr);
							if ((*it)->isOp(ptr))
								(*it)->deleteOp(ptr);
							if ((*it)->isMembersEmpty())
								delete_chan(data, *it);
							--it;
						}
					}
					// delete user from user vector list
					for(std::vector<user *>::iterator ite = data.users_vec.begin(); ite != data.users_vec.end(); ite++)
					{
						if (*ite == ptr)
						{
							delete ptr;
							data.users_vec.erase(ite);
							break ;
						}
					}
					break ;
				}
				// process the message that came in
				else
				{
					// buffer gestion
					buffer[valread] = '\0';
					if (buffer[valread - 1] != '\n')
					{
						ptr->setBuffer(ptr->getBuffer() + buffer);
						break ;
					}
					if (ptr->getBuffer().empty())
						ptr->setBuffer(buffer);
					else
						ptr->setBuffer(ptr->getBuffer() + buffer);

					// print the incoming message from a client
					//std::cout << YELLOW_CLR << ptr->getBuffer() << RESET_CLR << std::endl;

					// process commands to send the appropriate response
					cmd_processing(data, ptr, ptr->getBuffer());

					ptr->cleanBuffer();
				}
			}
		}
	}
	return (0);
}
