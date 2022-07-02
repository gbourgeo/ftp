/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_stor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/11 18:10:54 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/30 13:17:54 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/param.h>
#define __USE_POSIX199309
#include <time.h>
#include "sv_main.h"
#include "sv_struct.h"

int			sv_stor_exec(char *opt, char **cmds, t_client *cl)
{
	char	buff[DATA_BUFF_SIZE];
	int		ret;

	(void)opt;
	(void)cmds;
	while (1)
	{
		errno = 0;
		ret = recv(cl->data.sock_fd, buff, sizeof(buff),
			MSG_DONTWAIT | MSG_NOSIGNAL);
		if (ret == 0 || errno == ECONNRESET || errno == EPIPE)
			break ;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			nanosleep(&(struct timespec){ 0, 500000L }, NULL);
		else if (ret < 0)
			return (ERR_RECV);
		else if (write(cl->data.ffd, buff, ret) != ret)
			return (ERR_WRITE);
	}
	return (IS_OK);
}

static int	sv_stor_open_file(char *file, t_data *data, t_client *cl)
{
	char	full_path[MAXPATHLEN];
	char	*dup;
	int		ret;

	if (file[0] == '/')
		ft_strncpy(full_path, file, MAXPATHLEN);
	else
	{
		ft_strncpy(full_path, cl->pwd, MAXPATHLEN);
		ft_strncat(full_path, file, MAXPATHLEN);
	}
	ret = ERR_MALLOC;
	if ((dup = ft_strdup(full_path)) == NULL
	|| (ret = ft_check_path(&dup, cl->pwd, cl->home)) != IS_OK)
		return (ret);
	data->ffd = open(dup, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	free(dup);
	if (data->ffd < 0)
		return (ERR_OPEN);
	return (IS_OK);
}

/*
** STOR
** 125, 150
**   (110)
**   226, 250
**   425, 426, 451, 551, 552
** 532, 450, 452, 553
** 500, 501, 421, 530
*/

int			sv_stor(char **cmds, t_client *cl)
{
	int		errnb;

	cl->data.ffd = -1;
	cl->data.function = sv_stor_exec;
	if (GET_BIT(g_serv.options, sv_user_mode) && !cl->login.logged)
		return (sv_response(cl, "530 Please login with USER and PASS."));
	if (!sv_check_err(cl->errnb, sizeof(cl->errnb) / sizeof(cl->errnb[0])))
		return (sv_response(cl, "421 Closing connection"));
	if (!cmds[1] || !sv_validpathname(cmds[1]) || cmds[2])
		return (sv_response(cl, "501 %s", ft_get_error(ERR_INVALID_PARAM)));
	if (!cl->data.port && cl->data.pasv_fd < 0 && cl->data.sock_fd < 0)
		return (sv_response(cl, "425 Use PORT or PASV first"));
	if ((errnb = sv_stor_open_file(cmds[1], &cl->data, cl)) != IS_OK
	|| (errnb = sv_new_pid(cmds, cl, NULL)) != IS_OK
	|| (errnb = sv_response(cl, "125 Ready for transfert")) != IS_OK)
		errnb = sv_response(cl, "451 Internal error (%s)", ft_get_error(errnb));
	return (errnb);
}

/*
** STOR <SP> <pathname> <CRLF>
*/

int			sv_stor_help(t_command *cmd, t_client *cl)
{
	static char	*help[] = {
		"This command causes the server-DTP to accept the data",
		"transferred via the data connection and to store the data as",
		"a file at the server site.  If the file specified in the",
		"pathname exists at the server site, then its contents shall",
		"be replaced by the data being transferred.  A new file is",
		"created at the server site if the file specified in the",
		"pathname does not already exist.", NULL
	};

	return (sv_print_help(cl, cmd, "<pathname>", help));
}
