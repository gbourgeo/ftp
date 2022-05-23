/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_struct.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:07:30 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/04/13 14:01:59 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_STRUCT_H
# define CL_STRUCT_H

# include <ncurses.h>
# include "common.h"

/*
** Client Binary Options Structure
*/

typedef struct		s_client_options
{
	char			c;
	const char		*str;
	const char		*param;
	const char		*description;
	int				(*function)();
}					t_opt;

t_opt				*cl_params(int getsize);

typedef struct		s_client_parameters
{
	t_opt			*opts;
	char			*address;
	char			*port;
	size_t			size;
	int				i;
}					t_param;

/*
** Ncurses getch
*/

typedef struct		s_ncurse_character_read
{
	int				value;
	int				(*hdlr)();
}					t_read;

#endif
