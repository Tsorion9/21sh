/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anton <a@b>                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 01:48:28 by anton             #+#    #+#             */
/*   Updated: 2020/06/28 11:34:52 by anton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"

/*
** Split system path by ':' (Unescaped ':', probably)
** And Try to acess each dir and check for execution rights
** Return NULL in case of errors or full path to executable otherwise
*/

static char	*find_executable(char *name, char *env_path)
{
	char	**dirs;
	char	**start;
	char	*name_to_check;

	dirs = ft_strsplit(env_path, ':');
	start = dirs;
	if (!dirs--)
		return (NULL);
	while (*(++dirs))
	{
		if (!(name_to_check = ft_path_append(*dirs, name)))
			return (NULL);
		if (access(name_to_check, F_OK) != 0)
		{
			free(name_to_check);
			continue ;
		}
		del_array(start);
		return (name_to_check);
	}
	del_array(start);
	return (NULL);
}

/*
** Luckily, filenames cannot contain '/' in unix.
** That's why '/' in filename indicates path
*/

static int	looks_like_path(char *name)
{
	if (!*name)
		return (0);
	while (*name)
	{
		if (*name == '/')
			return (1);
		name++;
	}
	return (0);
}

/*
** Return a duplicate of path
** Or search in $PATH (if such exists)
*/

char		*find_path(char *name)
{
	char	*env_path;

	if (looks_like_path(name))
		return (ft_strdup(name));
	env_path = ft_getenv(g_env, "PATH");
	if (!env_path)
		return (NULL);
	return (find_executable(name, env_path));
}
