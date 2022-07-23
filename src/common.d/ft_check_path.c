/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 03:08:41 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/17 20:39:24 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "common.h"

static char		*setup_path(char *path, char *pwd)
{
	char		*tmp;
	char		*ret;
	int			do_free;

	do_free = 0;
	if (pwd[ft_strlen(pwd) - 1] != '/')
	{
		if (!(tmp = ft_strjoin(pwd, "/")))
			return (NULL);
		do_free = 1;
	}
	else
		tmp = pwd;
	if (!(ret = ft_strjoin(tmp, path)))
		return (NULL);
	if (do_free)
		free(tmp);
	return (ret);
}

/**
 * @brief Si "path" ne commence pas par '/' alors "pwd" et "path" sont
 * joins. Ensuite "path" est parsé afin d'enlever les '.' et '..'.
 * Enfin, "home" et "path" sont joins.
 * 
 * @param path Le chemin à recréer.
 * @param pwd Le répertoire courant.
 * @param home Le répertoire toujours joins avant "path".
 * @return int IS_OK (0) si tout s'est bien passé, ERR_* (> 0) si une erreur
 * a été rencontrée.
 */
int				ft_check_path(char **path, char *pwd, char *home)
{
	char		*pth;
	char		*tmp;
	int			do_setup;

	pth = *path;
	if (!pth || pth[0] == '-')
		return (IS_OK);
	do_setup = (pth[0] != '/');
	if (do_setup)
		if (!(pth = setup_path(*path, pwd)))
			return (ERR_MALLOC);
	pth = ft_recreate_path(pth);
	if (home[ft_strlen(home) - 1] != '/')
		tmp = ft_strjoin(home, pth);
	else
		tmp = ft_strjoin(home, pth + 1);
	if (do_setup)
		free(pth);
	if (!tmp)
		return (ERR_MALLOC);
	free(*path);
	*path = tmp;
	return (IS_OK);
}
