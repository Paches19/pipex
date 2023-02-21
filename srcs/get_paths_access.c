/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths_access.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:26:30 by adpachec          #+#    #+#             */
/*   Updated: 2023/02/21 10:39:41 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**get_path(char **envp)
{
	int		i;
	char	*env;
	char	**paths;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		++i;
	env = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	paths = ft_split(env, ':');
	free (env);
	return (paths);
}

int	get_size_cmd(char **cmd)
{
	int	size;

	size = 0;
	while (cmd[size])
		++size;
	return (size);
}

char	*try_access(char **cmd, char **paths)
{
	char	*file_path;
	int		err;
	int		i;

	err = -1;
	i = -1;
	file_path = NULL;
	while (paths[++i] && err < 0)
	{
		if (file_path)
			free(file_path);
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin_free(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[0]);
		err = access(file_path, X_OK);
	}
	if (err < 0)
	{
		free(file_path);
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	return (file_path);
}

char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, char **envp)
{
	char	*file_path;

	paths[0] = get_path(envp);
	cmd[0] = ft_split(argv[3], ' ');
	file_path = try_access(cmd[0], paths[0]);
	return (file_path);
}