/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_rmdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 19:15:11 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/30 12:24:41 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int				cl_rmdir(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	int		ret;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("RMD");
	new_list = cl_command_new(cmd, cl->ncu.chatwin, " 2");
	ret = cl_refresh_server_list_window(new_list, sv, cl);
	return (ret);
}

int				cl_rmdir_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the user to erase a directory in the server.",
		NULL
	};

	return (cl_help_print(cmd, "<directory_name>", help, cl));
}
