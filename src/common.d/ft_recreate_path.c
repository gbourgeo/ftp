/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recreate_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 22:39:34 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/14 13:22:28 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "common.h"

static char		*search_back(char *head, char *path)
{
	if (head > path)
		--head;
	while (head > path && *head != '/')
		--head;
	return (head);
}

/**
 * @brief Cette fonction recrée le chemin passé en argument en traitant les
 * '.' et '..' . Exemple:
 * "/home/work/./projet/test/../file.txt" -> "/home/work/projet/file.txt"
 * 
 * @param path Chemin à analyser et recréer si nécessaire.
 * @return char* Le chemin recréer.
 */
char			*ft_recreate_path(char *path)
{
	char	*head;
	char	*tail;

	head = path;
	while (*head && *(head + 1))
	{
		if (!(tail = ft_strchr(head + 1, '/')))
			tail = head + ft_strlen(head);
		if (!ft_strncmp(head + 1, ".", tail - head - 1))
			head = ft_strcpy(head, tail);
		else if (!ft_strncmp(head + 1, "..", tail - head - 1))
		{
			if ((head = search_back(head, path)))
				head = ft_strcpy(head, tail);
		}
		else
			head = tail;
	}
	if (!*path)
		ft_strcpy(path, "/");
	return (path);
}
