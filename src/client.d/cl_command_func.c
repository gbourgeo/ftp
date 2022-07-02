/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_command_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:24:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/02 10:56:03 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"
#include "cl_struct.h"

static char		*cl_tabtostr(char **tab)
{
	char	*ret;
	size_t	tab_size;
	size_t	i;

	tab_size = 0;
	i = 0;
	while (tab[i] != NULL)
	{
		tab_size += ft_strlen(tab[i]) + 1;
		i++;
	}
	ret = ft_memalloc(tab_size + 1);
	if (ret == NULL)
		return (ret);
	i = 0;
	while (tab[i] != NULL)
	{
		ft_strcat(ret, tab[i]);
		ft_strcat(ret, " ");
		i++;
	}
	ret[tab_size - 1] = '\n';
	return (ret);
}

t_cmd_l			*cl_command_new(char **command, WINDOW *win, char *codes)
{
	t_cmd_l	*cmd;

	if (!(cmd = ft_memalloc(sizeof(*cmd))))
		return (NULL);
	cmd->full_cmd = cl_tabtostr(command);
	cmd->ret_codes = ft_strdup(codes);
	if (cmd->full_cmd == NULL || cmd->ret_codes == NULL)
		return (cl_command_remove_elem(cmd, NULL));
	cmd->printtowin = win;
	cmd->data_socket_state = DATA_SOCKET_NOOPERATION;
	return (cmd);
}

static void			cl_remove_elem(t_cmd_l *cmd)
{
	if (cmd == NULL)
		return ;
	ft_strdel(&cmd->full_cmd);
	ft_strdel(&cmd->ret_codes);
	free(cmd);
}

t_cmd_l			*cl_command_remove_elem(t_cmd_l *cmd, t_cmd_l *cmd_list)
{
	if (cmd->list.elem.next)
	{
		cmd->list.elem.next->prev = cmd->list.elem.prev;
		if (cmd->list.list.next)
			cmd->list.list.next->prev = cmd->list.elem.next;
		cmd_list = (t_cmd_l *)cmd->list.elem.next;
	}
	else
	{
		if (cmd->list.list.next)
			cmd->list.list.next->prev = cmd->list.elem.prev;
		cmd_list = (t_cmd_l *)cmd->list.list.next;
	}
	cl_remove_elem(cmd);
	return (cmd_list);
}

t_cmd_l			*cl_command_remove_list(t_cmd_l *cmd, t_cmd_l *cmd_list)
{
	t_cmd_l		*save;

	if (cmd->list.elem.next)
		(void)cl_command_remove_list((t_cmd_l *)cmd->list.elem.next, cmd_list);
	save = (t_cmd_l *)cmd->list.list.next;
	cl_remove_elem(cmd);
	return (save);
}
