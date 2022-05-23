/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_client_history.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:50:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/18 13:47:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "get_next_line.h"
#include "cl_main.h"

int				cl_history_init(t_client *cl)
{
	char		*line;
	int			count;
	int			fd;

	line = NULL;
	count = 0;
	fd = open(CL_HIST_FILE, O_CREAT | O_RDONLY, 0644);
	if (fd < 0)
		return (ERR_OPEN);
	while (get_next_line(fd, &line) > 0 && count < CL_HIST_SIZE)
	{
		if (line && *line)
		{
			cl->hist = cl_history_new(line, cl->hist);
			count++;
		}
		else
			ft_strdel(&line);
	}
	cl->hist = cl_history_new(NULL, cl->hist);
	ft_strdel(&line);
	close(fd);
	return (IS_OK);
}

t_hist			*cl_history_new(char *line, t_hist *hist)
{
	t_hist		*new;

	if (!(new = ft_memalloc(sizeof(*new))))
		return (hist);
	new->line = line;
	new->list.next = (void *)hist;
	if (hist)
		hist->list.prev = (void *)new;
	return (new);
}

static int		cl_history_check_duplicate(char *line, t_hist *hist)
{
	t_hist		*head;

	if ((head = hist))
		while (hist)
		{
			if (!ft_strcmp(line, hist->line))
			{
				if (hist->list.prev)
					((t_hist *)hist->list.prev)->list.next = hist->list.next;
				if (hist->list.next)
					((t_hist *)hist->list.next)->list.prev = hist->list.prev;
				if ((hist->list.next = head->list.next))
					((t_hist *)head->list.next)->list.prev = (void *)hist;
				head->list.next = (void *)hist;
				hist->list.prev = (void *)head;
				return (0);
			}
			hist = (void *)hist->list.next;
		}
	return (1);
}

t_hist			*cl_history_add(char *line, t_hist *hist)
{
	t_hist		*ptr;
	t_hist		*next;
	int			count;

	count = 1;
	while (hist && hist->list.prev)
		hist = (void *)hist->list.prev;
	if (hist && cl_history_check_duplicate(line, hist))
	{
		ft_strdel(&hist->line);
		hist->line = ft_strdup(line);
		ptr = hist;
		while (ptr)
			if (count++ > CL_HIST_SIZE && ptr->list.next)
			{
				ft_strdel(&((t_hist *)ptr->list.next)->line);
				next = (void *)ptr->list.next->next;
				free(ptr->list.next);
				ptr->list.next = (void *)next;
			}
			else
				ptr = (void *)ptr->list.next;
		hist = cl_history_new(NULL, hist);
	}
	return (hist);
}
