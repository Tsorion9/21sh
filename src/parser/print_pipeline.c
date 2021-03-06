/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsance <jsance@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 21:39:03 by jsance            #+#    #+#             */
/*   Updated: 2021/03/06 21:39:04 by jsance           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*get_cmd_type(t_command_type type)
{
	if (type == SIMPLE_CMD)
		return ("SIMPLE_CMD");
	else if (type == BRACE_GROUP)
		return ("BRACE_GROUP");
	else if (type == SUBSHELL)
		return ("SUBSHELL");
	else
		return ("UNKNOWN");
}

void	print_command(t_command *command)
{
	ft_printf("\e[0;33m%17s = %s\n", "cmd type",
				get_cmd_type(command->cmd_type));
	if (command->cmd_type == SIMPLE_CMD)
		print_simple_cmd(command->simple_cmd);
	else if (command->cmd_type == BRACE_GROUP)
		print_brace_group(command->brace_group);
	else
		print_subshell(command->subshell);
}

void	print_pipeline(t_pipeline *pipeline)
{
	t_pipeline *tmp;

	tmp = pipeline;
	while (tmp)
	{
		print_command(tmp->command);
		tmp = tmp->next;
	}
}
