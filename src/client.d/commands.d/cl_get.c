/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_get.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 18:17:26 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/23 11:03:17 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "cl_main.h"

static int		cl_get_open_file(char **cmd, int *fd)
{
	char		*file;
	char		*newfile;

	file = ft_strrchr(cmd[1], '/');
	file = (file == NULL) ? cmd[1] : newfile + 1;
	newfile = (char *)ft_memalloc(ft_strlen(file) + ft_strlen(cmd[2]) + 2);
	if (newfile == NULL)
		return (ERR_MALLOC);
	if (cmd[2])
	{
		ft_strcpy(newfile, cmd[2]);
		ft_strcat(newfile, "/");
	}
	newfile = ft_strcat(newfile, file);
	*fd = open(newfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	ft_strdel(&newfile);
	if (*fd < 0)
		return (ERR_OPEN);
	return (IS_OK);
}

/**
 * @brief Récupère un fichier distant
 * 
 * @param cmd Totalité de la commande :
 * [0]	Mot clé de la commande
 * [1]	Fichier à récupérer sur le serveur (impératif)
 * [2]	Répertoire de copie du fichier à récupérer (optionnel).
 * @param sv Structure du serveur à contacter.
 * @param cl Structure du client.
 * @return int IS_OK sans erreur, ERR_* si une erreur a été rencontrée.
 */
int				cl_get(char **cmd, t_server *sv, t_client *cl)
{
	t_cmd_l	*new_list;
	t_cmd_l	*new_elem;
	int		errnb;

	if (sv == NULL)
		return (ERR_NO_SERVER);
	if (cmd[1] == NULL)
		return (ERR_NB_PARAMS);
	errnb = cl_get_open_file(cmd, &sv->filefd);
	if (errnb != IS_OK)
		return (errnb);
	ft_strdel(&cmd[0]);
	ft_strdel(&cmd[2]);
	cmd[0] = ft_strdup("RETR");
	new_list = cl_command_new((char *[]){ "PASV", NULL }, cl->ncu.chatwin, " 2");
	new_elem = cl_command_new(cmd, cl->printtowin, " 12");
	new_elem->data_socket_state = DATA_SOCKET_RECEIVE;
	new_list = elem_insert_tail(new_elem, new_list);
	errnb = cl_refresh_server_list_window(new_list, sv, cl);
	return (errnb);
}

int				cl_get_help(t_command *cmd, t_client *cl)
{
	static char		*help[] = {
		"This command allows the user to receive a file from the server.",
		NULL
	};

	return (cl_help_print(cmd, "<filename / filepath> [destination path]", help, cl));
}
