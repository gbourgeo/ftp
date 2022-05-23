/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_params_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/05 22:36:17 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/05/01 16:08:09 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cl_main.h"
#include "cl_struct.h"

static int		only_digit(const char *s)
{
	while (*s)
		if (!ft_isdigit(*s++))
			return (0);
	return (1);
}

static int		get_value(char **av, t_param *p, int *i, t_client *cl)
{
	size_t		j;
	size_t		k;
	int			errnb;

	j = 0;
	while (av[*i][++j])
	{
		k = -1;
		while (++k < p->size)
			if (av[*i][j] == p->opts[k].c
			|| (p->opts[k].str
				&& ft_strequ(&av[*i][j], p->opts[k].str)))
			{
				if ((errnb = p->opts[k].function(av, i, cl)) != IS_OK)
					return (errnb);
				if (p->opts[k].param
				|| (p->opts[k].str
					&& ft_strequ(&av[*i][j], p->opts[k].str)))
					return (IS_OK);
				break ;
			}
		if (k == p->size)
			return (ERR_WRONG_PARAM);
	}
	return (IS_OK);
}

static void		init_param(t_param *p)
{
	p->opts = cl_params(0);
	p->size = (size_t)cl_params(1);
	p->address = NULL;
	p->port = NULL;
}

static int		check_params(t_param *p, t_client *cl)
{
	if (p->address == NULL || p->port == NULL)
		return (ERR_NB_PARAMS);
	if (!only_digit(p->port))
		return (ERR_DIGIT_PARAM);
	cl->cmd_list = cl_command_new((char *[]){
		"connect", p->address, p->port, NULL },
		cl->ncu.chatwin,
		" ");
	return (IS_OK);
}

int				cl_params_get(char **av, t_client *cl)
{
	t_param		p;
	int			i;
	int			errnb;

	init_param(&p);
	i = 1;
	errnb = IS_OK;
	while (av[i] && errnb == IS_OK)
	{
		if (av[i][0] == '-')
			errnb = get_value(av, &p, &i, cl);
		else if (p.address == NULL)
			p.address = av[i];
		else if (p.port == NULL)
			p.port = av[i];
		else
			errnb = ERR_TOOMUCH_PARAM;
		i++;
	}
	if (errnb != IS_OK)
		return (errnb);
	return (check_params(&p, cl));
}
