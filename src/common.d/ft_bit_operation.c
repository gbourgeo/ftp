/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bit_operation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 18:52:39 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/03/13 16:46:29 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

/*
** Retourne la valeur du bit à la position pos du nombre num
*/
int			ft_get_bit(int num, int pos)
{
	return (num & (1 << pos));
}

/*
** Affecte le bit au nombre num à la position pos.
** Si value vaut 0 le bit est retiré, s'il vaut 1 le bit est mis.
*/
int			ft_set_bit(int num, int pos, char value)
{
	if (value == 0)
		return (num ^ (1 << value));
	return (num | (1 << pos));
}
