/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_params.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 18:02:17 by gbourgeo          #+#    #+#             */
/*   Updated: 2021/12/26 12:52:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

t_opt			*sv_params(int getsize)
{
	static t_opt	opts[] = {
		{ '4', NULL, NULL, "Allows IpV4 connections only.", sv_param_four },
		{ '6', NULL, NULL, "Allows IpV6 connections only.", sv_param_six },
		{ 'i', NULL, NULL, "Interactive server (default: off).", sv_param_i },
		{ 'p', "-path", "[path]", "Server working directory (default: '.').", sv_param_p },
		{ 'u', "-user", NULL, "Registered users mode (default: off).", sv_param_u },
		{ 'd', "-dir", NULL, "Users directory created (with -u).", sv_param_d },
		{ 'h', "-help", NULL, "Print help and exit.", sv_param_h },
	};

	if (getsize)
		return (t_opt *)(sizeof(opts) / sizeof(opts[0]));
	return (opts);
}
