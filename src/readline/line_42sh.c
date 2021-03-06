/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_42sh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mphobos <mphobos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 10:41:09 by mphobos           #+#    #+#             */
/*   Updated: 2021/03/08 10:41:12 by mphobos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "readline.h"
#include "exec.h"

char	*line_42sh(char *prompt)
{
	char	*line;
	int		status;

	if (g_interactive_shell)
		return (readline(prompt));
	status = get_next_line_wrapper(STDIN_FILENO, &line);
	if (status == -1)
		ft_fprintf(STDERR_FILENO, "%s\n", "42sh: read error");
	return (line);
}
