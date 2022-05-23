/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_response.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 19:17:13 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/18 13:17:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include "cl_main.h"

static int	convert(char str[], char **addr, size_t addr_len, char **p_port)
{
	size_t			i;
	unsigned short	port;
	char			*nb1;
	char			*nb2;

	i = 0;
	if ((*addr = ft_memalloc(addr_len + 1)) == NULL)
		return (ERR_MALLOC);
	while (str[i] && i < addr_len)
	{
		(*addr)[i] = str[i];
		if (str[i] == ',')
			(*addr)[i] = (addr_len == INET_ADDRSTRLEN) ? '.' : ':';
		i++;
	}
	nb2 = ft_strrchr(*addr, (addr_len == INET_ADDRSTRLEN) ? '.' : ':');
	*nb2++ = '\0';
	nb1 = ft_strrchr(*addr, (addr_len == INET_ADDRSTRLEN) ? '.' : ':');
	*nb1++ = '\0';
	port = ft_atoi(nb1) << 8;
	port = port + ft_atoi(nb2);
	*p_port = ft_itoa(port);
	return ((*p_port == NULL) ? ERR_MALLOC : IS_OK);
}

int			cl_connect_back(t_server *sv)
{
	char		*addr;
	size_t		addr_len;
	char		*port;
	int			errnb;

	addr_len = (sv->ip_version == AF_INET) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
	if ((errnb = convert(sv->response + 5, &addr, addr_len, &port)) == IS_OK)
		errnb = cl_connect_to(&sv->fd_data, addr, port, NULL);
	ft_strdel(&addr);
	ft_strdel(&port);
	return (errnb);
}

int			cl_response(t_server *sv)
{
	int				errnb;

	errnb = IS_OK;
	if (!ft_strchr(sv->response, '\n') || !is_valid_response(sv->response))
		return (IS_OK);
	if (sv->response[0] > '2')
	{
		cl_server_close_data(sv);
		return (IS_OK);
	}
	if (ft_atoi(sv->response) == 227)
		errnb = cl_connect_back(sv);
	return (errnb);
}
