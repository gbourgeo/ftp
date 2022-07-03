/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_main.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:46:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/07/03 13:30:05 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_MAIN_H
# define CL_MAIN_H

# include <signal.h>
# include <ncurses.h>
# include <sys/types.h>
# include "libft.h"
# include "common.h"
# include "lists.h"

# define CL_HIST_SIZE	3
# define CL_HIST_FILE	".ftphist"

/*
** Enumeration for CLIENT options
*/
enum
{
	cl_verbose = 0,
};

/*
** Ncurses related enum and structure
*/
enum
{
	CLIENT_DFLT_BCKGD = 1,
	CLIENT_COLOR_RED,
	CLIENT_DFLT_COLOR,
	CLIENT_TITLE_COLOR,
	CLIENT_TEXT_COLOR,
	CL_RED,
	CL_GREEN,
	CL_BLUE,
	CL_BACK_RED,
	CL_BACK_GREEN,
};

/*
** DATA socket states
*/
typedef enum
{
	DATA_SOCKET_NOOPERATION = 0,
	DATA_SOCKET_SEND,
	DATA_SOCKET_RECEIVE,
} e_data_socket_state;

typedef struct		s_ncurse
{
	WINDOW			*main;
	WINDOW			*chatbox;
	WINDOW			*chatwin;
	WINDOW			*slistbox;
	WINDOW			*slistwin;
	WINDOW			*clistbox;
	WINDOW			*clistwin;
	WINDOW			*textbox;
	WINDOW			*textwin;
}					t_ncu;

/*
** History structure
*/
typedef struct		s_history
{
	t_clist			list;
	char			*line;
}					t_hist;

/*
** Command to be launched at starting loop structure
** codes : Si le code est un espace, lance la commande de wr, sinon attends la réponse du serveur.
**         Si le premier chiffre est supérieur à celui attendu, toutes les precommande sont effacées,
**         sinon on passe au chiffre suivant.
*/
typedef struct		s_command_list
{
	t_celem				list;
	char				*full_cmd;
	char				*ret_codes;
	WINDOW				*printtowin;
	e_data_socket_state	data_socket_state;
	void				*sv;
}					t_cmd_l;


/*
** Server related structure
*/
typedef struct		s_server
{
	t_clist				list;
	char				*address;
	char				*port;
	char				*username;
	char				*password;
	int					ip_version;
	int					fd_ctrl;
	int					fd_data;
	char				*filename;
	int					filefd;
	char				*working_dir;
	t_buff				rd;
	t_buff				wr;
	t_cmd_l				*cmd_list;
	int					errnb[6];
	char				response[CMD_BUFF_SIZE];
}					t_server;

typedef void		(*t_sighandler)(int);

/*
** Client main structure (global)
*/
typedef struct		s_client
{
	t_common		info;
	t_sighandler	sig[NSIG];
	t_ncu			ncu;
	WINDOW			*printtowin;
	t_cmd_l			*cmd_list;
	t_server		*server_list;
	int				errnb[2];
	t_buff			rd;
	t_hist			*hist;
	int				options;
}					t_client;

struct s_client		g_cl;

/*
** Client functions
*/
int					cl_client_commands(char *cmd, t_server *sv, t_client *cl);
int					cl_client_signals(t_client *cl);
int					cl_params_get(char **av, t_client *cl);
int					cl_param_i(char **av, int *i, t_client *cl);
int					cl_param_h(char **av, int *i, t_client *cl);
int					cl_param_n(char **av, int *i, t_client *cl);

int					cl_init(char **environ, t_client *cl);
int					cl_ncurses_init(t_client *cl);
int					create_s_text(t_client *cl);
int					create_s_list(t_client *cl);
int					create_c_list(t_client *cl);
int					create_c_text(t_client *cl);

int					cl_history_init(t_client *cl);
t_hist				*cl_history_new(char *line, t_hist *hist);
t_hist				*cl_history_add(char *line, t_hist *hist);

int					cl_connect_to_server(t_server *sv, t_client *cl);
int					cl_connect_to(int *fd, char *addr, char *port, int *ip_v);
int					cl_get_userinfo(t_server *sv, t_client *cl);
int					cl_get_username(t_server *sv, t_client *cl);
int					cl_get_userpass(t_server *sv, t_client *cl);

void				cl_client_end(t_client *cl);
void				cl_ncurses_end(t_client *cl);
void				cl_history_end(t_hist *hist);

int					cl_client_loop(t_client *cl);
int					cl_client_pid(pid_t pid, t_client *cl);
void				cl_ncurses_copy(char *s);
int					cl_ncurses_read(t_buff *ring, t_client *cl);
int					cl_ncurses_write(t_buff *ring, t_server *sv, t_client *cl);
t_server			*cl_server_new(const char *addr, const char *port);
void				cl_server_close_data(t_server *sv);
t_server *			cl_server_close(t_server *sv, t_server *sv_list);
int					cl_server_recv(t_buff *ring, int fd);
int					cl_server_send(t_buff *ring, int fd);
int					cl_server_recv_data(t_server *sv, t_client *cl);
int					cl_server_send_data(t_server *sv, t_client *cl);
int					cl_server_write(const char buf[], t_server *sv);

t_cmd_l				*cl_command_new(char **command, WINDOW *win, char *codes);
t_cmd_l				*cl_command_remove_elem(t_cmd_l *cmd, t_cmd_l *cmd_list);
t_cmd_l				*cl_command_remove_list(t_cmd_l *cmd, t_cmd_l *cmd_list);
int					cl_command_exec_cl(t_client *cl);
int					cl_command_exec_sv(t_server *sv, t_client *cl);

t_command			*cl_commands(int getsize);
int					cl_response(t_server *sv);
char				*cl_ringbuffcat(char *buff, int size, t_buff *ring);

int					cl_connect_back(t_server *sv);

int					cl_refresh_server_list_window(t_cmd_l *first_elem,
t_server *sv, t_client *cl)
;

/*
** Commands
*/
int					cl_bslash(char **cmd, t_client *cl);
int					cl_bslash_cd(char **cmd, t_client *cl);
int					cl_cd(char **cmd, t_server *sv, t_client *cl);
int					cl_clear(char **cmd, t_server *sv, t_client *cl);
int					cl_connect(char **cmd, t_server *sv, t_client *cl);
int					cl_exit(char **cmd, t_server *sv, t_client *cl);
int					cl_get(char **cmd, t_server *sv, t_client *cl);
int					cl_help(char **cmd, t_server *sv, t_client *cl);
int					cl_help_local(char **cmd, t_server *sv, t_client *cl);
int					cl_ls(char **cmd, t_server *sv, t_client *cl);
int					cl_mkdir(char **cmd, t_server *sv, t_client *cl);
int					cl_nlst(char **cmd, t_server *sv, t_client *cl);
int					cl_pass(char **cmd, t_server *sv, t_client *cl);
int					cl_put(char **cmd, t_server *sv, t_client *cl);
int					cl_pwd(char **cmd, t_server *sv, t_client *cl);
int					cl_quit(char **cmd, t_server *sv, t_client *cl);
int					cl_refresh(char **cmd, t_server *sv, t_client *cl);
int					cl_rm(char **cmd, t_server *sv, t_client *cl);
int					cl_rmdir(char **cmd, t_server *sv, t_client *cl);
int					cl_user(char **cmd, t_server *sv, t_client *cl);

int					cl_bslash_help(t_command *cmd, t_client *cl);
int					cl_cd_help(t_command *cmd, t_client *cl);
int					cl_clear_help(t_command *cmd, t_client *cl);
int					cl_connect_help(t_command *cmd, t_client *cl);
int					cl_exit_help(t_command *cmd, t_client *cl);
int					cl_get_help(t_command *cmd, t_client *cl);
int					cl_help_help(t_command *cmd, t_client *cl);
int					cl_help_loc_help(t_command *cmd, t_client *cl);
int					cl_help_print(t_command *cmd, char *args, char **descri,
					t_client *cl);
int					cl_ls_help(t_command *cmd, t_client *cl);
int					cl_mkdir_help(t_command *cmd, t_client *cl);
int					cl_nlst_help(t_command *cmd, t_client *cl);
int					cl_pass_help(t_command *cmd, t_client *cl);
int					cl_put_help(t_command *cmd, t_client *cl);
int					cl_pwd_help(t_command *cmd, t_client *cl);
int					cl_quit_help(t_command *cmd, t_client *cl);
int					cl_refresh_help(t_command *cmd, t_client *cl);
int					cl_rm_help(t_command *cmd, t_client *cl);
int					cl_rmdir_help(t_command *cmd, t_client *cl);
int					cl_user_help(t_command *cmd, t_client *cl);

/*
** Ncurses keys
*/
int					cl_ctrl_c(t_buff *ring, t_client *cl);
int					cl_ctrl_d(t_buff *ring, t_client *cl);
int					cl_lf(t_buff *ring, t_client *cl);
int					cl_backspace(t_buff *ring, t_client *cl);
int					cl_key_dc(t_buff *ring, t_client *cl);
int					cl_key_up(t_buff *ring, t_client *cl);
int					cl_key_down(t_buff *ring, t_client *cl);
int					cl_key_left(t_buff *ring, t_client *cl);
int					cl_key_right(t_buff *ring, t_client *cl);

/*
** Utils
*/
int					is_valid_response(char res[]);

#endif
