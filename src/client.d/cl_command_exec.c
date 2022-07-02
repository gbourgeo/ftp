/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_command_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 15:46:03 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/02 09:16:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int			cl_command_exec_cl(t_client *cl)
{
	t_cmd_l		*cmd;
	t_server	*sv;
	int			errnb;

	cmd = cl->cmd_list;
	errnb = IS_OK;
	if (cmd == NULL)
		return (IS_OK);
	sv = (t_server *)cmd->sv;
	if (cmd->ret_codes[0] == ' ')
	{
		if ((cl->printtowin = cmd->printtowin) != cl->ncu.chatwin)
			wclear(cmd->printtowin);
		wrefresh(cl->printtowin);
		errnb = cl_client_commands(cmd->full_cmd, sv, cl);
		ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
	}
	else if (sv && ft_strchr(sv->response, '\n'))
	{
		if (is_valid_response(sv->response))
		{
			if (sv->response[0] - cmd->ret_codes[0] > 0)
			{
				cl->cmd_list = cl_command_remove_list(cmd, cl->cmd_list);
				return (IS_OK);
			}
			ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
		}
		ft_strclr(sv->response);
	}
	if (cmd->ret_codes[0] == '\0')
		cl->cmd_list = cl_command_remove_elem(cmd, cl->cmd_list);
	return (errnb);
}

int			cl_command_exec_sv(t_server *sv, t_client *cl)
{
	t_cmd_l	*cmd;
	int		errnb;

	if (sv == NULL || sv->cmd_list == NULL)
		return (IS_OK);
	cmd = sv->cmd_list;
	errnb = IS_OK;
	if (cmd->ret_codes[0] == ' ')
	{
		if ((cl->printtowin = cmd->printtowin) != cl->ncu.chatwin)
			wclear(cmd->printtowin);
		wrefresh(cl->printtowin);
		errnb = cl_server_write(cmd->full_cmd, sv);
		ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
		ft_strclr(sv->response);
	}
	else if (ft_strchr(sv->response, '\n'))
	{
		if (is_valid_response(sv->response))
		{
			if (sv->response[0] - cmd->ret_codes[0] > 0)
			{
				sv->cmd_list = cl_command_remove_list(cmd, sv->cmd_list);
				cl_server_close_data(sv);
				return (IS_OK);
			}
			else if (ft_strncmp(sv->response, "227 ", 4) == 0)
				errnb = cl_connect_back(sv);
			else if (ft_strncmp(sv->response, "125 ", 4) == 0)
			{
				if (ft_strncmp(cmd->full_cmd, "STOR ", 5) == 0)
					cmd->data_socket_state = DATA_SOCKET_SEND;
				else if (ft_strncmp(cmd->full_cmd, "RETR ", 5) == 0)
					cmd->data_socket_state = DATA_SOCKET_RECEIVE;
			}
			ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
		}
		ft_strclr(sv->response);
	}
	if (cmd->ret_codes[0] == '\0')
		sv->cmd_list = cl_command_remove_elem(cmd, sv->cmd_list);
	return (errnb);
}
