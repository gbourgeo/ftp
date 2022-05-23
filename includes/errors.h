/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/01 13:38:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/01/01 13:39:52 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
* Errors strings corresponding to the errors enum (common.h)
*/
static const char	*g_errors[] = {
		"Bye.",
		"-- Help",
		"Missing parameter",
		"Wrong parameter",
		"Invalid parameter",
		"parameter contains non digit characters",
		"too much paramaters given",
		"getaddrinfo failed",
		"port already in use (v4)",
		"port already in use (v6)",
		"setsockopt failed",
		"listen(AF_INET) failed",
		"open failed",
		"listen(AF_INET6) failed",
		"malloc failed",
		"select failed",
		"accept IP_V4 failed",
		"accept IP_V6 failed",
		"recv failed",
		"disconnected from server",
		"signal received",
		"wait failed",
		"fork failed",
		"dup2 failed",
		"execv failed",
		"send failed",
		"invalid username/password",
		"failed to change working directory",
		"invalid command",
		"user already registered",
		"write failed",
		"users file not found",
		"get_next_line failed",
		"connection failed",
		"Unable to open port",
		"DATA header corrupted",
		"transfert timed out",
		"lseek failed",
		"mmap failed",
		"fstat failed",
		"not valid file",
		"no server found",
		"ncurse: init main win failed",
		"ncurse: init text box failed",
		"ncurse: init text win failed",
		"ncurse: init list box failed",
		"ncurse: init list win failed",
		"ncurse: init chat box failed",
		"ncurse: init chat win failed",
		"read failed",
		"open terminal failed",
		"tcgetattr failed",
		"tcsetattr failed",
		"Quit",
		"Specify a terminal type",
		"Could not access the termcap data base",
		"Terminal type is not defined",
		"pipe failed",
		"Already connected to a server",
};
