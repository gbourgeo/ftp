/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:18:43 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/03 13:49:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "cl_main.h"

static int		cl_put_open_file(char *filepath, t_server *sv, t_env *env)
{
	int			errnb;

	ft_close(&sv->filefd);
	if (!(sv->filename = ft_strdup(filepath)))
		return (ERR_MALLOC);
	if ((errnb = ft_check_path(&sv->filename, env->pwd, "/")) == IS_OK)
	{
		if ((sv->filefd = open(sv->filename, O_RDONLY)) < 0)
			errnb = ERR_OPEN;
	}
	ft_strdel(&sv->filename);
	return (errnb);
}

static int		cl_put_get_dest_path(char **cmd, char *sv_working_dir)
{
	char	*cmd_1;
	char	*cmd_2;
	char	*dest;
	int		len;

	if ((cmd_1 = ft_strrchr(cmd[1], '/')) == NULL)
		cmd_1 = cmd[1];
	if ((cmd_2 = cmd[2]) == NULL)
		cmd_2 = sv_working_dir;
	dest = (char *)ft_memalloc(ft_strlen(cmd_1) + ft_strlen(cmd_2) + 2);
	if (dest == NULL)
		return (ERR_MALLOC);
	ft_strcpy(dest, cmd_2);
	if ((len = ft_strlen(dest)) == 0)
		len = 1;
	if (dest[len - 1] != '/' && cmd_1[0] != '/')
		ft_strcat(dest, "/");
	ft_strcat(dest, cmd_1);
	free(cmd[1]);
	cmd[1] = dest;
	return (IS_OK);
}

int				cl_put(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	t_cmd_l	*new_elem;
	int		errnb;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	if (cmd[1] == NULL)
		return (ERR_NB_PARAMS);
	errnb = cl_put_open_file(cmd[1], sv, &cl->info.env);
	if (errnb != IS_OK)
		return (errnb);
	free(cmd[0]);
	cmd[0] = ft_strdup("STOR");
	errnb = cl_put_get_dest_path(cmd, sv->working_dir);
	if (errnb != IS_OK)
		return (errnb);
	ft_strdel(&cmd[2]);
	new_list = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, " 2");
	new_elem = cl_command_new(cmd, cl->printtowin, " 12");
	new_list = elem_insert_tail(new_elem, new_list);
	errnb = cl_refresh_server_list_window(new_list, sv, cl);
	return (errnb);
}

int				cl_put_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the user to send a file to the server.", NULL
	};

	return (cl_help_print(cmd, "<file_name / file_path> [destination_path]", help, cl));
}
