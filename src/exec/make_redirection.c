#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "exec.h"

static void make_heredoc_redirection(t_redirect *redirect)
{
	int pipefd[2];
	pid_t child;

	pipe(pipefd);
	dup2(pipefd[0], redirect->redirector->fd);
	close_wrapper(pipefd[0]);
	child = fork();
	if (child)
	{
		close_wrapper(pipefd[1]);
		return ;
	}
	else
	{
		close_wrapper(redirect->redirector->fd);
		ft_fprintf(pipefd[1], "%s", redirect->heredoc_value);
		exit(0);
	}
}

static int file_redirection(t_redirect *redirect, int flags, int mode)
{
	int fd;

	fd = open(redirect->redirector->filename, flags, mode);
	if (fd == -1)
	{
		ft_fprintf(STDERR_FILENO, "%s %s\n",
				"Could not open file:", redirect->redirector->filename);
		return (1);
	}
	dup2(fd, redirect->redirector->fd);
	close_wrapper(fd);
	return (0);
}

int make_redirection(t_redirect *redirect)
{
	if (redirect->instruction == INPUT_DIRECTION)
		file_redirection(redirect, O_RDONLY, 0666);
	else if (redirect->instruction == OUTPUT_DIRECTION)
		file_redirection(redirect, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (redirect->instruction == DOUBLE_OUTPUT_DIRECTION)
		file_redirection(redirect, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (redirect->instruction == DOUBLE_INPUT_DERECTION)
		make_heredoc_redirection(redirect);
	if (redirect->instruction == DUPLICAT_OUTPUT ||
			redirect->instruction == DUPLICAT_INPUT)
	{
		if (!ft_strcmp(redirect->redirector->filename, "-"))
		{
			close_wrapper(redirect->redirector->fd);
			return (1);
		}
		dup2(ft_atoi(redirect->redirector->filename), redirect->redirector->fd); // TODO: read/write 0 bytes to check if fd is valid
		//close_wrapper(redirect->redirector->fd);
	}
	return (0);
}
