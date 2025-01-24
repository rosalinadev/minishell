/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvandepu <rvandepu@student.42lehavre.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:59:38 by rvandepu          #+#    #+#             */
/*   Updated: 2024/12/08 17:19:45 by rvandepu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "defaults.h"
# include "libft.h"

typedef enum e_setsig
{
	S_INTERACTIVE,
	S_HEREDOC,
	S_IGNORE,
	S_DEFAULT,
}	t_setsig;

# define EXIT_PARSER_FAILURE 2
# define EXIT_EXEC_ERROR 126
# define EXIT_NOT_FOUND 127
# define EXIT_SIG 128

typedef enum e_eno
{
	E__NOPRINT = -1,
	E__UNKNOWN = 0,
	E_MEM,
	E_OPEN,
	E_EXP_CMD_PIPE,
	E_PIPE_EXP_CMD,
	E_HD_EXP_DELIM,
	E_RD_EXP_FILENAME,
	E_QUOTES,
	E_ARGS_IDENTIFIER,
	E_ARGS_TOO_FEW,
	E_ARGS_TOO_MANY,
	E_ARGS_NUMERIC,
	E_EXECVE,
	E_PIPE,
	E_FORK,
	E_WRITE,
	E_CHDIR,
	E_CMD_NOT_FOUND,
	E_CMD_IS_DIR,
	E_HOMEUNSET,
	E_HD_NO_DELIM,
	E__MAX
}	t_eno;

typedef struct s_err
{
	t_eno	eno;
	int		errsv;
}	t_err;

typedef struct s_env
{
	struct s_env	*next;
	char			*var;
	char			*val;
}	t_env;

typedef struct s_redir
{
	char	*filename;
	bool	is_heredoc;
	bool	read_heredoc;
	bool	quoted;
	bool	append;
}	t_redir;

typedef struct s_cmd
{
	t_redir	redir[2];
	int		argc;
	char	**argv;
	pid_t	pid;
}	t_cmd;

typedef struct s_ctx
{
	t_err	err;
	t_env	*env;
	int		cmd_count;
	t_cmd	*cmds;
	bool	should_exit;
	uint8_t	exitcode;
	bool	is_child;
	void	(*debug_hook)(struct s_ctx *);
}	t_ctx;

# define WHITESPACE " \t"

typedef enum e_parse
{
	P_INVALID = 0,
	P_COMMAND,
	P_ARGUMENT,
	P_DOUBLEQUOTE,
	P_HEREDOC,
	P__MAX,
}	t_parse;

typedef struct s_parser
{
	t_ctx	*ctx;
	t_redir	*redir;
	t_redir	*parsing_redir;
	char	**cmdline;
	bool	has_skipped;
	t_parse	type;
	int		count;
	char	**tokens;
}	t_parser;

// minishell.c
void	free_cmds(t_ctx *ctx);

// signals.c
extern int		g_last_sig;
void	set_signals(t_setsig set);

// error.c
void	err_p(const char *s, t_err *err);
void	err_p_clear(const char *s, t_err *err);
void	eno(t_ctx *ctx, t_eno eno);
void	enosv(t_ctx *ctx, t_eno errnum, int errsv);

// env.c
bool	env_set(t_env **env, char *var);
bool	env_del(t_env **env, char *name);
char	*env_get(t_env *env, char *name);
char	**env_environ(t_env *start);

// env_utils.c
int		_env_namecmp(char *var1, char *var2);
bool	env_init(t_ctx *ctx, char **environ);
void	env_clear(t_env **env);

/*
 * Parsing
 */

// parser.c
bool	parse_tokens(t_parser *parser, int depth);
int		parse_cmdline(t_ctx *ctx, char *cmdline, int depth);

// parser_utils.c
void	free_cmd(t_cmd *cmd);
char	*dup_token(char *src, int len);
void	get_parser(t_parser *parser, t_parser *parent, t_parse type);
void	free_parser(t_parser *parser);
bool	sub_parser(t_parser *parent, t_parse type, char **token);

// parse_{command,argument,var,doublequote}.c
bool	parse_command(t_parser *parser, char **token);
bool	parse_argument(t_parser *parser, char **token);
bool	parse_var(t_parser *parser, char **token);
bool	parse_doublequote(t_parser *parser, char **token);

// heredoc.c
bool	read_heredocs(t_ctx *ctx);
bool	parse_heredoc(t_parser *parser, char **token);

/*
 * Executing
 */

typedef enum e_bt
{
	BT__INVALID = 0,
	BT__FIRST,
	BT_DBG_CMDS = BT__FIRST,
	BT_ECHO,
	BT_CD,
	BT_PWD,
	BT_EXPORT,
	BT_UNSET,
	BT_ENV,
	BT_EXIT,
	BT__MAX
}	t_bt;

typedef bool	(*t_bt_f)(t_ctx *ctx, t_cmd *cmd);

// exec.c
bool	execute(t_ctx *ctx, t_cmd *cmd);
bool	exec_cmds(t_ctx *ctx);

// exec_utils.c
int		ft_close(int *fd);
void	closetab(int count, int *fd_tab);
void	free_tab(char **tab);
bool	handle_redirection(t_ctx *ctx, t_cmd *cmd);

// builtin.c
t_bt_f	get_builtin(t_cmd *cmd);
void	run_builtin(t_ctx *ctx, t_cmd *cmd);
bool	exec_builtin_nofork(t_ctx *ctx, t_cmd *cmd);

// bt_{debug_cmds,echo,cd,pwd,export,unset,env,exit}.c
bool	bt_dbg_cmds(t_ctx *ctx, t_cmd *cmd);
bool	bt_echo(t_ctx *ctx, t_cmd *cmd);
bool	bt_cd(t_ctx *ctx, t_cmd *cmd);
bool	bt_pwd(t_ctx *ctx, t_cmd *cmd);
bool	bt_export(t_ctx *ctx, t_cmd *cmd);
bool	bt_unset(t_ctx *ctx, t_cmd *cmd);
bool	bt_env(t_ctx *ctx, t_cmd *cmd);
bool	bt_exit(t_ctx *ctx, t_cmd *cmd);

#endif
