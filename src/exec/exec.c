#include <sys/types.h>
#include <unistd.h>

#include "libft.h"
#include "lexer.h"
#include "parser.h"
#include "exec.h"
#include "job.h"

/* 
** top-level shell is the only one who does job control
** if top_level_shell==0, we are in job shell
** all our children must stay in same group
*/
int top_level_shell = 1;

void sigchld_handler(int n)
{
	pid_t child;
	int status;

	child = wait(&status);
	if (WIFSTOPPED(status))
	{
		ft_printf("%d Stopped\n", child);
	}
	if (WIFCONTINUED(status))
	{
		ft_printf("%d Continued\n", child);
	}
	if (WIFEXITED(status))
	{
		ft_printf("%d Terminated\n", child);
	}

	//remove_job(child);
}

/*
** TRUE = 0
** FALSE = 1
** unlike C language, weird shell stuff
*/
static void update_status(int pipline_status, int *status, t_type_andor last_op)
{
	if (last_op == ANDOR_NONE)
	{
		*status = pipline_status; /* First step */
	}
	else if (last_op == ANDOR_AND)
	{
		*status = *status || pipline_status;
	}
	else if (last_op == ANDOR_OR)
	{
		*status = *status && pipline_status;
	}
}

/*
** Return 1 in case of continue
*/
static int need_exec_pipeline(int status, t_type_andor last_op)
{
	if (last_op == ANDOR_NONE || 
		(last_op == ANDOR_OR && status == 0) || 
		(last_op == ANDOR_AND && status != 0))
	{
		return (1);
	}
	return (0);
}

void set_jobshell_signal(void)
{
	signal(SIGINT, SIG_IGN); /* In case of come child handles */
	signal(SIGTERM, SIG_IGN); /* In case of come child handles */
	signal(SIGCHLD, SIG_IGN); /* We wait, parent does job control */
	signal(SIGTSTP, SIG_DFL);
}

void perform_word_expansions(t_pipeline *p)
{
	return ;
}

/*
** Return 1
*/
void exec_andor_list(t_andor_list *list, int *status)
{
	int				pipline_status;
	t_type_andor	last_op;

	last_op = ANDOR_NONE;
	while (list)
	{
		if (need_exec_pipeline(*status, last_op))
		{
			perform_word_expansions(list->pipeline); /* Only executed commands should be expanded 
			TODO: Make sure expansions are executed single time
			*/
			pipline_status = exec_pipline(list->pipeline);
			update_status(pipline_status, status, last_op);
		}
		last_op = list->type_andor;
		list = list->next;
	}
}

/*
** Return exit status
*/
int exec_complete_cmd(t_complete_cmd *cmd)
{
	int				status;
	t_complete_cmd	*save_start;
	pid_t			job;

	signal(SIGCHLD, &sigchld_handler);
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	save_start = cmd;
	while (cmd)
	{
		if (cmd->separator_op == OP_BG) 
		{
			job = fork();
			if (job) /* top-level shell */
			{
				setpgid(job, job);
				add_job(job, 1);
				cmd = cmd->next;
				continue ;
			}
			else	 /* job shell */
			{
				setpgid(getpid(), getpid());
				set_jobshell_signal();
				top_level_shell = 0;
				exec_andor_list(cmd->and_or, &status);
				exit(status);
			}
		}
		exec_andor_list(cmd->and_or, &status);
		cmd = cmd->next;
	}

	clean_complete_command(&save_start);
	return (status);
}
