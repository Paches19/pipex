/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/07 11:24:24 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_management(int err)
{
	write(2, "ERROR: ", 7);
	perror(strerror(err));
	exit (-1);	
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
		error_management(ERANGE);
	ptr = (void *) malloc(len);
	if (!ptr)
		error_management(ENOMEM);
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
		error_management(ENXIO);
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
		error_management(ENOMEM);
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
		while (*s != c && *s != '\0' && temp++ > 0)
			++s;
		if (temp > 0)
		{
			res[j] = ft_substr(s - temp, 0, temp);
			if (!res[j++])
			{
				ft_free_matrix(res);
				error_management(ENOMEM);
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
		error_management(ENXIO);
	words = ft_words(s, c);
	res = (char **) ft_calloc(sizeof(char *), (words + 1));
	if (!res)
		error_management(ENOMEM);
	ft_init_matrix(s, c, res, words);
	if (!res)
		error_management(ENOMEM);
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		error_management(ENXIO);
	len_s1 = ft_strlen(s1);
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 1));
	if (!str)
		error_management(ENOMEM);
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
	if (ft_strncmp(envp[i], "PATH=", 5))
		error_management(ENOENT);
	env = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	paths = ft_split(env, ':');
	free (env);
	return (paths);
}

char	**get_cmd(char *const *argv)
{
	char	*av;
	char	**cmd;

	av = ft_strjoin(argv[2], " ");
	av = ft_strjoin(av, argv[3]);
	cmd = ft_split(av, ' ');
	free (av);
	return (cmd);
}

char	*try_access(char **cmd, char **paths, int *n_cmd)
{
	char	*file_path;
	int		err;
	int		i;

	while (cmd[*n_cmd][0] == '-')
		++*n_cmd;
	err = -1;
	i = -1;
	while (paths[++i] && err < 0)
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[*n_cmd]);
		err = access(file_path, X_OK);
	}
	if (err < 0)
		error_management(err);
	return (file_path);
}

int	get_size_cmd(char **cmd, int n_cmd)
{
	int	size;

	size = 1;
	while (cmd[++n_cmd] && cmd[n_cmd][0] == '-')
		++size;
	return (size);
}

char **get_av(char **cmd, int n_cmd)
{
	char	**av;
	int		size_cmd;
	int		i;

	size_cmd = get_size_cmd(cmd, n_cmd);
	av = (char **)ft_calloc(sizeof(char *), size_cmd + 1);
	i = -1;
	while (++i < size_cmd)
	{
		av[i] = (char *)ft_calloc(sizeof (char), ft_strlen(cmd[i + n_cmd]) + 1);
		if (!av[i])
		{
			ft_free_matrix(av);
			error_management(ENOMEM);
		}
	}
	av[0] = cmd[n_cmd];
	i = 1;
	while (cmd[n_cmd + i] && cmd[n_cmd + i][0] == '-')
	{
		av[i] = cmd[n_cmd + i];
		++i;
	}
	return (av);
}

void	first_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		n_cmd;

	paths = get_path(envp);
	cmd = get_cmd(argv);
	n_cmd = 0;
	file_path = try_access(cmd, paths, &n_cmd);
	cmd = get_av(cmd, n_cmd);
	n_cmd = close(fd1[READ_END]);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = dup2(fd1[WRITE_END], STDOUT_FILENO);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = execve(file_path, cmd, NULL);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = close(fd1[WRITE_END]);
	if (n_cmd < 0)
		error_management(n_cmd);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
}

char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, char **envp)
{
	int		n_cmd;
	char	*file_path;

	paths[0] = get_path(envp);
	cmd[0] = get_cmd(argv);
	n_cmd = 1;
	file_path = try_access(cmd[0], paths[0], &n_cmd);
	cmd[0] = get_av(cmd[0], n_cmd);
	return (file_path);
}

void	second_son(int fd1[2], int fd_exit, char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		n_cmd;

	file_path = get_paths_cmd_son_2(&paths, &cmd, argv, envp);
	fd_exit = open(argv[4], O_WRONLY);
	if (fd_exit < 0)
		error_management(fd_exit);
	n_cmd = dup2(fd1[READ_END], STDIN_FILENO);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = close(fd1[READ_END]);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = dup2(fd_exit, STDOUT_FILENO);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = execve(file_path, cmd, NULL);
	if (n_cmd < 0)
		error_management(n_cmd);
	n_cmd = close (fd_exit);
	if (n_cmd < 0)
		error_management(n_cmd);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
}

void	pipex(char *const *argv, char **envp)
{
	int		fd1[2];
	int		fd_exit;
	pid_t	pid;
	int		status;
	int		errno;

	errno = pipe(fd1);
	if (errno < 0)
		error_management(errno);
	pid = fork();
	if (pid < 0)
		error_management(pid);
	if (!pid)
		first_son(fd1, argv, envp);
	else
	{
		wait(&status);
		if (status < 0)
			exit (-1);
		close(fd1[WRITE_END]);
		pid = fork();
		fd_exit = 0;
		if (!pid)
			second_son(fd1, fd_exit, argv, envp);
		else
			close(fd1[READ_END]);
	}
	wait(&status);
}

void	check_cmd(char **argv)
{
	int	n_cmd;
	int	i;
	char **cmd;

	cmd = get_cmd(argv);
	i = 0;
	n_cmd = 1;
	while (cmd[++i])
	{
		if (cmd[i][0] != '-')
			++n_cmd;
	}
	if (n_cmd > 2)
		error_management(EINVAL);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		error_management(EINVAL);
	//check_cmd(argv);
	pipex(argv, envp);
}
