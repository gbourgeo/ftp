/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_client_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/26 16:58:33 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/06/28 11:37:04 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

static int		cl_client_exec_cmd(char **cmd_t, t_server *sv, t_client *cl)
{
	t_command	*cmds;
	long		i;

	cmds = cl_commands(0);
	i = 0;
	if (cmd_t[0][0] == '\\')
		return (cl_bslash(cmd_t, cl));
	while (i < (long)cl_commands(1))
	{
		if (!ftp_strcmp(cmds[i].name, cmd_t[0]))
			return (cmds[i].func(cmd_t, sv, cl));
		i++;
	}
	if (sv == NULL)
		return (ERR_NO_SERVER);
	t_cmd_l *new_cmd = cl_command_new(cmd_t, cl->ncu.chatwin, " 2");
	sv->cmd_list = list_insert_tail(new_cmd, sv->cmd_list);
	return (IS_OK);
}

int				cl_client_commands(char *cmd, t_server *sv, t_client *cl)
{
	char		**cmd_table;
	int			errnb;

	wprintw(cl->ncu.chatwin, "> %s", cmd);
	wrefresh(cl->ncu.chatwin);
	if (!(cmd_table = ft_split_whitespaces(cmd)))
		return (ERR_MALLOC);
	errnb = IS_OK;
	if (cmd_table[0] && cmd_table[0][0])
		errnb = cl_client_exec_cmd(cmd_table, sv, cl);
	ft_tabdel(&cmd_table);
	return (errnb);
}
