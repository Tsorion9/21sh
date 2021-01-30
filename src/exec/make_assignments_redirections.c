#include "exec.h"

int make_assignments_redirections(t_simple_cmd *cmd)
{
	t_redirect *redirect;

	make_assignments(cmd);
	redirect = cmd->redirects;
	while (redirect)
	{
		if (make_redirection(redirect) != 0)
			return (1);
		redirect = redirect->next;
	}
	return (0);
}
