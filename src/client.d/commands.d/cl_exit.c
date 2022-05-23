/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 16:35:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/12 00:52:52 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int					cl_exit(char **cmd, t_server *sv, t_client *cl)
{
	(void)cmd;
	(void)sv;
	(void)cl;
	return (ERR_QUIT);
}

int					cl_exit_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allws the user to quit clearly the server and ",
		"the client.", NULL
	};

	return (cl_help_print(cmd, "", help, cl));
}
