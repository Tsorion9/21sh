#include <unistd.h>
#include <stdio.h>
#include "expansions.h"
#include "parser.h"
#include "exec.h"

static char				*concat_and_free(t_list **l, unsigned int total_len)
{
	char			*c;
	size_t			i;

	i = 0;
	c = 0;
	if (total_len + 1)
		c = (char *)malloc(total_len + 1);
	while (*l)
	{
		ft_memcpy(c + i, (const void *)(*l)->content, (*l)->content_size);
		i += (*l)->content_size;
		ft_lstdelone(l, &del_simple);
	}
	c[i] = 0;
	return (c);
}

char				*read_from_pipe(int fd)
{
	static char		buf[1002];
	t_list			*l;
	size_t			bytes_read;
	size_t			total_len;

	bytes_read = 0;
	total_len = 0;
	l = NULL;
	while ((bytes_read = read(fd, buf, 1000)) > 0)
	{
		total_len += bytes_read;
		l = ft_lstappend(l, buf, bytes_read);
	}
	return (concat_and_free(&l, total_len));
}

void					command_substitution(char **s, int word_state)
{
	t_complete_cmd	*cmd;
	char			*tmp;
	pid_t			child;
	int				pipefd[2];

	tmp = ft_strdup(*s);
	cmd = parser(&tmp);
//	print_complete_command(cmd);
	if (!cmd)
	{
		ft_fprintf(2, "%s\n", E_CMD_BAD_SUBSTITUTION);
		expasnion_status(EXPANSION_FAIL);
		return ;
	}
	pipe(pipefd);
	child = fork();
	if (child) /* Parent */
	{
		close_wrapper(pipefd[1]);
		free(*s);
		free_lexer_state(&g_token);
		*s = read_from_pipe(pipefd[0]);
		clean_complete_command(&cmd);
		close_wrapper(pipefd[0]);
	}
	else /* Child */
	{
		//sleep(1);
		close_wrapper(pipefd[0]);
		//close_wrapper(STDIN_FILENO);
		dup2(pipefd[1], STDOUT_FILENO);
		close_wrapper(pipefd[1]);
		interactive_shell = 0;
		exec_complete_cmd(cmd);
		while ((tmp != NULL && *tmp != '\0'))
		{
			cmd = parser(&tmp);
			if (cmd == NULL)
				break ;
			exec_complete_cmd(cmd);
		}
		exit(0);
	}
	if (!(word_state & IN_DQUOTE_STATE) && !(word_state & IN_QUOTE_STATE))
		expasnion_status(NEED_FIELD_SPLIT);
}
