#ifndef TASK_CONTEXT_H
# define TASK_CONTEXT_H

# define IGNORE_STREAM -1

typedef struct	s_task_context
{
	int			in_pipe;
	int			out_pipe;
	int			save_0;
	int			save_1;
	int			need_child;
	int			need_subshell;
}				t_task_context;

#endif