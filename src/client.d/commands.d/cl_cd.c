/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:15:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/30 12:22:16 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int			cl_cd(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	int		ret;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	free(cmd[0]);
	cmd[0] = ft_strdup("CWD");
	new_list = cl_command_new(cmd, cl->ncu.chatwin, " 2");
	ret = cl_refresh_server_list_window(new_list, sv, cl);
	return (ret);
}

int				cl_cd_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the client to change the server",
		"working directory.", NULL
	};

	return (cl_help_print(cmd, "[<directory_name>]", help, cl));
}
