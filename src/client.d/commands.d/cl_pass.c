/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_pass.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/17 17:24:25 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/30 12:23:45 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int					cl_pass(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	int		ret;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("PASS");
	new_list = cl_command_new(cmd, cl->ncu.chatwin, " 2");
	ret = cl_refresh_server_list_window(new_list, sv, cl);
	return (ret);
}

int					cl_pass_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command sends to the server the password needed to log in",
		"if the server needs it. This command MUST be preceded by the ",
		"command USER.", NULL
	};

	return (cl_help_print(cmd, "<password>", help, cl));
}
