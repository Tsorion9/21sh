/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_assignments.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsance <jsance@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 15:17:30 by jsance            #+#    #+#             */
/*   Updated: 2021/03/07 15:17:31 by jsance           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	only_assignments(t_pipeline *pipeline)
{
	if (pipeline->next)
	{
		return (0);
	}
	if (pipeline->command->cmd_type == SIMPLE_CMD)
	{
		if (pipeline->command->simple_cmd->words)
		{
			return (0);
		}
	}
	return (1);
}
