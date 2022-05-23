/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/20 08:14:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/01/01 13:39:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef __linux__
# define _POSIX_C_SOURCE 200809L
#endif
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "common.h"
#include "errors.h"
#include "libft.h"

void				ft_print_error(int errnb, t_common *info)
{
	const char	*err;

	if ((err = ft_get_error(errnb)))
	{
		if (info)
		{
			dprintf(STDERR_FILENO, "%s: %s", info->progname, err);
			if (errno)
				dprintf(STDERR_FILENO, ": %s", strerror(errno));
			dprintf(STDERR_FILENO, ".\n");
		}
	}
}

const char			*ft_get_error(int errnb)
{
	if (errnb < 0 || errnb > (int)(sizeof(g_errors) / sizeof(g_errors[0])))
		return (NULL);
	return (g_errors[errnb]);
}
