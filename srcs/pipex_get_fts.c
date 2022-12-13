/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_get_fts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 16:57:48 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/13 16:58:41 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
			paths[i] = ft_strjoin(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[0]);
		err = access(file_path, X_OK);
	}
	if (err < 0)
	{
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 20);
		free(file_path);
		exit(127);
	}
	return (file_path);
}

char	**get_av(char **cmd)
{
	char	**av;
	int		size_cmd;
	int		i;

	size_cmd = get_size_cmd(cmd);
	av = (char **)ft_calloc(sizeof(char *), size_cmd + 1);
	i = -1;
	while (++i < size_cmd)
	{
		av[i] = (char *)ft_calloc(sizeof (char), ft_strlen(cmd[i]) + 1);
		if (!av[i])
		{
			ft_free_matrix(av);
			error_management();
		}
	}
	av[0] = cmd[0];
	i = 0;
	while (cmd[++i])
		av[i] = cmd[i];
	return (av);
}

char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, \
char **envp)
{
	char	*file_path;

	paths[0] = get_path(envp);
	cmd[0] = ft_split(argv[3], ' ');
	file_path = try_access(cmd[0], paths[0]);
	cmd[0] = get_av(cmd[0]);
	return (file_path);
}
