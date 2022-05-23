/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_command_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 15:46:03 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/01 16:06:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"

int			cl_command_exec_cl(t_client *cl)
{
	t_cmd_l		*cmd;
	t_server	*sv;
	int			errnb;

	cmd = cl->cmd_list;
	errnb = NOT_DEFINED;
	if (cmd == NULL)
		return (NOT_DEFINED);
	sv = (t_server *)cmd->sv;
	if (cmd->ret_codes[0] == ' ')
	{
		if ((cl->printtowin = cmd->printtowin) != cl->ncu.chatwin)
			wclear(cmd->printtowin);
		errnb = cl_client_commands(cmd->full_cmd, sv, cl);
		ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
		if (sv)
			ft_strclr(sv->response);
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
	// else if (!sv)
	// 	cl->cmd_list = cl_command_remove_elem(cmd, cl->cmd_list);
	if (cmd->ret_codes[0] == '\0')
		cl->cmd_list = cl_command_remove_elem(cmd, cl->cmd_list);
	return (errnb);
}

int			cl_command_exec_sv(t_server *sv, t_client *cl)
{
	t_cmd_l	*cmd;
	int		errnb;

	if (sv == NULL || sv->cmd_list == NULL)
		return (NOT_DEFINED);
	cmd = sv->cmd_list;
	errnb = NOT_DEFINED;
	if (cmd->ret_codes[0] == ' ')
	{
		if ((cl->printtowin = cmd->printtowin) != cl->ncu.chatwin)
			wclear(cmd->printtowin);
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
			ft_strcpy(cmd->ret_codes, cmd->ret_codes + 1);
		}
		ft_strclr(sv->response);
	}
	if (cmd->ret_codes[0] == '\0')
		sv->cmd_list = cl_command_remove_elem(cmd, sv->cmd_list);
	return (errnb);
}
