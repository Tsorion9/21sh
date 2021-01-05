/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nriker <nriker@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 20:16:51 by anton             #+#    #+#             */
/*   Updated: 2021/01/05 11:49:30 by nriker           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc21sh.h"
#include "static_env.h"
#include "readline.h"
#include "t_hashalias.h"

void	repl(int tty_input)
{
	t_deque		*command;

	(void)tty_input;
	command = NULL;
	if (isatty(STDIN_FILENO))
	{
		rp(init_rp());
		load_on_file_history(rp(NULL)->history);
	}
	while (1)
	{
		fuck_checklist_signal_state(1, 0);
		command = parser();
		tc_clear_till_end();
		if (command)
			last_cmd_status(1, exec_cmd(command));
	}
	save_in_file_history(rp(NULL)->history);
	if (isatty(STDIN_FILENO))
		free_rp();
}

int		main(int ac, char **av, char **environ)
{
	int	tty_input;
	int	fd;

	if (ac > 1)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			ft_fprintf(2, "21sh: Error! Could not open file: %s\n", av[1]);
			exit(1);
		}
		dup2(fd, 0);
		close(fd);
	}
	static_env_action(init, (void *)environ);
	static_hashalias_action(init);
	if ((tty_input = isatty(STDIN_FILENO)))
	{
		init_terminal();
		init_prompt();
	}
	// set_signal();
	repl(tty_input);
	return (0);
}
