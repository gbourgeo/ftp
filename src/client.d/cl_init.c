/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 04:51:10 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/13 18:18:19 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "cl_main.h"

int				cl_init(char **environ, t_client *cl)
{
	char		*ptr;
	size_t		i;

	if ((ptr = ft_getenv("HOME=", environ)))
	{
		ft_strdel(&cl->info.env.home);
		cl->info.env.home = ptr;
	}
	if (!(cl->info.env.pwd = ft_getenv("PWD=", environ)))
		cl->info.env.pwd = getcwd(NULL, 0);
	if (!(cl->info.env.oldpwd = ft_getenv("OLDPWD=", environ)))
		cl->info.env.oldpwd = getcwd(NULL, 0);
	if (!cl->info.env.home || !cl->info.env.pwd || !cl->info.env.oldpwd)
		return (ERR_MALLOC);
	cl->printtowin = cl->ncu.chatwin;
	cl->rd.head = cl->rd.buff;
	cl->rd.tail = cl->rd.buff;
	cl->rd.len = 0;
	i = 0;
	while (i < sizeof(cl->errnb) / sizeof(cl->errnb[0]))
		cl->errnb[i++] = NOT_DEFINED;
	return (IS_OK);
}
