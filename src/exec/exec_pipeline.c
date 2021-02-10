#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "inc21sh.h"
#include "libft.h"
#include "exec.h"
#include "t_builtin.h"
#include "find_exec.h"
#include "job.h"
#include "assignment_word.h"
#include "expand_pipeline.h"
#include "pipeline_words_to_assignments.h"

static int exec_pipeline_job(t_pipeline *pipeline)
{
	int fd[2];
	int read_fd;
	pid_t last_child;
	int first_iter = 1;

	if (!pipeline)
		exit(last_cmd_status);
	fd[0] = IGNORE_STREAM;
	while (pipeline && pipeline->command)
	{
		read_fd = fd[0];
		fd[1] = IGNORE_STREAM;
		if (pipeline->next)
		{
			pipe(fd);
		}
		last_child = make_child(pipeline->command, read_fd, fd[1], fd[0], first_iter);
		first_iter = 0;
		pipeline = pipeline->next;
	}
	return (wait_all_children(last_child));
}

/*
** Wait for foreground job, from top_level_shell
*/
int wait_fg_job(pid_t job)
{
	int status;
	t_job *j;

	waitpid(job, &status, WUNTRACED);

	j = find_job(job);
	j->state = job_status_to_state(status);
	j->status = status;
	if (j->state != DONE)
	{
		j->priority = next_priority();
	}
	if (j->state == DONE)
	{
		remove_job(job);
	}

	if ((j = find_job(job)) && interactive_shell)
	{
		tcgetattr(STDIN_FILENO, &(j->tmodes));
	}
	if (interactive_shell)
	{
		/* Put top-level shell to foreground*/
		tcsetpgrp(STDIN_FILENO, getpid());
		set_shell_input_mode();
	}
	return (status);
}

static t_word_list *get_tail(t_word_list *list)
{
	t_word_list *tmp;

	tmp = list;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

static void	apply_field_splitting_simple_cmd(t_simple_cmd **simple_cmd)
{
	t_word_list *words;
	t_word_list *tail;
	t_word_list *head;
	t_word_list *fields;

	words = (*simple_cmd)->words;
	tail = NULL;
	head = NULL;
	while (words)
	{
		fields = field_splitting_list(words->word);
		if (head == NULL)
		{
			head = fields;
			tail = get_tail(fields);
		}
		else
		{
			tail->next = fields;
			tail = get_tail(fields);
		}
		words = words->next;
	}
	clean_words(&(*simple_cmd)->words);
	(*simple_cmd)->words = head;
}

static void apply_field_splitting(t_command *command)
{
	if (command->cmd_type == SIMPLE_CMD)
		apply_field_splitting_simple_cmd(&command->simple_cmd);
}

int exec_pipeline(t_pipeline *pipeline)
{
	pid_t job;

	if (expand_pipeline(pipeline) == EXPANSION_FAIL)
		return (1);
	pipeline_words_to_assignments(pipeline);
	apply_field_splitting(pipeline->command);
	if (is_single_builtin(pipeline) || only_assignments(pipeline))
		return (exec_single_builtin(pipeline));
	if (!top_level_shell)
		return (exec_pipeline_job(pipeline));
	job = fork();
	if (job) /* Top-level shell */
	{
		add_job(job, 0, get_pipeline_str(pipeline));
		if (interactive_shell)
		{
			setpgid(job, job);
			tcsetpgrp(STDIN_FILENO, job);
		}
		return (wait_fg_job(job)); /* Job is in foreground */
	}
	else /* Job shell */
	{
		if (interactive_shell)
		{
			setpgid(getpid(), getpid());
			tcsetpgrp(STDIN_FILENO, getpid()); /* We are foreground */
			set_jobshell_signal();
		}
		top_level_shell = 0;
		exit(exec_pipeline_job(pipeline));
	}
}
