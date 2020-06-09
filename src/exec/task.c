#include "exec_utils.h"
#include "make_assignments_redirections.h"
#include "static_env.h"

/*
** Return zero-terminated array of strings assuming the words are held in deque
** cmd->wl
*/

static char	**collect_argwords(t_simple_cmd *cmd)
{
	char	**args;
	int		i;

	i = 0;
	if (!deque_len(cmd->wl))
		return (NULL);
	args = malloc(sizeof(char *) * (deque_len(cmd->wl) + 1));
	while ((args[i] = pop_front(cmd->wl)))
		i++;
	free(cmd->wl);
	cmd->wl = NULL;
	return (args);
}


int	task(t_simple_cmd *cmd, t_task_context *task_context)
{
	char		**av;
	int			status;
	t_builtin	builtin;

	enter_task_context(task_context);
	make_assignments_redirections(cmd);
	if (deque_len(cmd->wl) == 0)
	{
		exit_task_context(task_context);
		rm_simple_cmd(cmd);
		return (0);
	}
	av = collect_argwords(cmd);
	if (!av)
		return (1);
	if (!task_context->need_child && (builtin = get_builtin(av[0])))
		status = builtin(av + 1, static_env_action(get, NULL));
	else
		find_exec(av, static_env_action(get, NULL));	
	del_array(av);
	rm_simple_cmd(cmd);
	exit_task_context(task_context);
	return (status);
}
