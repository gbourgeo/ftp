/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_rm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:19:49 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/23 16:52:24 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int				cl_rm(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	int		ret;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("DELE");
	new_list = cl_command_new(cmd, cl->ncu.chatwin, " ");
	ret = cl_refresh_server_list_window(new_list, sv, cl);
	return (ret);
}

int				cl_rm_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the user to erase a file in the server.", NULL
	};

	return (cl_help_print(cmd, "<file_name>", help, cl));
}
