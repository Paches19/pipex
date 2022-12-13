/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/13 12:50:37 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_management(void)
{
	int	err;

	err = errno;
	printf("\nerrnno: %d\n", err);
	perror(strerror(err));
	exit(err);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *) s;
	i = -1;
	while (++i < n && n > 0)
		ptr[i] = 0;
}

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	const size_t	len = count * size;

	if (!count)
		return (malloc(0));
	if (len / count != size)
		error_management();
	ptr = (void *) malloc(len);
	if (!ptr)
		error_management();
	ft_bzero(ptr, len);
	return (ptr);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	i;
	size_t			len_s;

	if (!s)
		error_management();
	len_s = ft_strlen(s);
	if (start >= len_s)
	{
		str = (char *) malloc(sizeof(char) * 1);
		str[0] = '\0';
		return (str);
	}
	if (len > len_s)
		str = (char *) malloc(sizeof(char) * (len_s + 1));
	else
		str = (char *) malloc(sizeof(char) * (len + 1));
	if (!str)
		error_management();
	i = -1;
	while (++i < len && s[start + i])
		str[i] = s[start + i];
	str[i] = '\0';
	return (str);
}

static size_t	ft_words(const char *s, char c)
{
	size_t	i;
	size_t	words;

	words = 0;
	if (s[0] == '\0')
		return (0);
	i = 0;
	if (s[0] != c)
	{
		++words;
		++i;
	}
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			++words;
		++i;
	}
	return (words);
}

static void	ft_free_matrix(char **matrix)
{
	size_t	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		++i;
	}
	free(matrix);
}

static void	ft_init_matrix(const char *s, char c, char **res, size_t words)
{
	size_t	j;
	size_t	temp;

	j = 0;
	while (*s != '\0' && j <= words)
	{
		temp = 0;
		while (*s == c)
			++s;
		while (*s != c && *s != '\0')
		{
			++temp;
			++s;
		}
		if (temp > 0)
		{
			res[j] = ft_substr(s - temp, 0, temp);
			if (!res[j++])
			{
				ft_free_matrix(res);
				error_management();
			}
		}
	}
	res[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**res;

	if (!s)
		error_management();
	words = ft_words(s, c);
	res = (char **) ft_calloc(sizeof(char *), (words + 1));
	if (!res)
		error_management();
	ft_init_matrix(s, c, res, words);
	if (!res)
		error_management();
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		error_management();
	len_s1 = ft_strlen(s1);
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 1));
	if (!str)
		error_management();
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		++i;
	}
	i = 0;
	while (s2[i])
	{
		str[len_s1 + i] = s2[i];
		++i;
	}
	str[len_s1 + i] = '\0';
	return (str);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char) s1[i] > (unsigned char) s2[i])
			return (1);
		else if ((unsigned char) s1[i] < (unsigned char) s2[i])
			return (-1);
		else if (s1[i] == '\0' || s2[i] == '\0')
			return (0);
		++i;
	}
	return (0);
}

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
	while (paths[++i] && err < 0)
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[0]);
		err = access(file_path, X_OK);
	}
	printf("\nerraccess: %d\n", err);
	if (err < 0)
	{
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 20);
		//free(file_path);
		//return (NULL);
		//error_management();
	}
	return (file_path);
}

int	get_size_cmd(char **cmd)
{
	int	size;

	size = 0;
	while (cmd[size])
		++size;
	return (size);
}

char **get_av(char **cmd)
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

char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, char **envp)
{
	char	*file_path;

	paths[0] = get_path(envp);
	cmd[0] = ft_split(argv[3], ' ');
	file_path = try_access(cmd[0], paths[0]);
	cmd[0] = get_av(cmd[0]);
	return (file_path);
}

void	first_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_input;

	fd_input = open(argv[1], O_RDONLY);
	if (fd_input < 0)
		error_management();
	paths = get_path(envp);
	cmd = ft_split(argv[2], ' ');
	file_path = try_access(cmd, paths);
	cmd = get_av(cmd);
	dup2(fd_input, STDIN_FILENO);
	close(fd_input);
	close(fd1[READ_END]);
	dup2(fd1[WRITE_END], STDOUT_FILENO);
	err = execve(file_path, cmd, NULL);
	printf("erno antes: %d", errno);
	close(fd1[WRITE_END]);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
	if (err < 0)
		error_management();
}

void	second_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_exit;

	fd_exit = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRWXU);
	if (fd_exit < 0)
		error_management();
	file_path = get_paths_cmd_son_2(&paths, &cmd, argv, envp);
	dup2(fd1[READ_END], STDIN_FILENO);
	close(fd1[READ_END]);
	dup2(fd_exit, STDOUT_FILENO);
	err = execve(file_path, cmd, NULL);
	close (fd_exit);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
	if (err < 0)
		error_management();
}

void	pipex(char *const *argv, char **envp)
{
	int		fd1[2];
	pid_t	pid;
	int		status;
	int		err;

	status = 0;
	err = pipe(fd1);
	if (err < 0)
		error_management();
	pid = fork();
	if (pid < 0)
		error_management();
	if (!pid)
		first_son(fd1, argv, envp);
	else
	{
		close(fd1[WRITE_END]);
		pid = fork();
		if (!pid)
			second_son(fd1, argv, envp);
		else
			close(fd1[READ_END]);
	}
	waitpid(pid, &status, 0);
	printf("status: %d\n", WEXITSTATUS(status));
	if (status != 0)
		exit(WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		write(2, "ERROR: not enough arguments\n", 29);
		write(2, "ARG: file_input \n", 29);
		exit (1);
	}
	pipex(argv, envp);
	return (0);
}
