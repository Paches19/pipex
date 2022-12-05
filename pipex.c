/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/05 11:30:52 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		return (NULL);
	ptr = (void *) malloc(len);
	if (!ptr)
		return (NULL);
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
		return (NULL);
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
		return (NULL);
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

static void	ft_free_res(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		res[i] = NULL;
		++i;
	}
	free(res);
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
		while (*s != c && *s != '\0' && temp++ >= 0)
			++s;
		if (temp > 0)
		{
			res[j] = ft_substr(s - temp, 0, temp);
			if (!res[j++])
			{
				ft_free_res(res);
				return ;
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
		return (NULL);
	words = ft_words(s, c);
	res = (char **) ft_calloc(sizeof(char *), (words + 1));
	if (!res)
		return (NULL);
	ft_init_matrix(s, c, res, words);
	if (!res)
		return (NULL);
	return (res);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	str = (char *) malloc(sizeof(char) * (len_s1 + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
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

void	error_management(void)
{
	printf("ERROR\n");
	exit (-1);
}

char	**get_path(char **envp)
{
	int		i;
	char	*env;
	char	**paths;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		++i;
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

char	*try_access(char **cmd, char **paths, int n_cmd)
{
	char	*file_path;
	int		err;
	int		i;

	if (cmd[n_cmd][0] == '-')
		++n_cmd;
	err = -1;
	i = -1;
	while (paths[++i] && err < 0)
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin(paths[i], "/");
		file_path = ft_strjoin(paths[i], cmd[n_cmd]);
		err = access(file_path, X_OK);
	}
	return (file_path);
}

void	first_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	char	**av;
	int		size_cmd;

	paths = get_path(envp);
	cmd = get_cmd(argv);
	file_path = try_access(cmd, paths, 0);
	close(fd1[READ_END]);
	dup2(fd1[WRITE_END], STDOUT_FILENO);
	close(fd1[WRITE_END]);
	av = (char **)ft_calloc(sizeof(char *), 3);
	av[0] = cmd[0];
	if (cmd[1][0] == '-')
		av[1] = cmd[1];
	execve(file_path, av, NULL);
	free (av);
}

int	get_size_cmd(char **cmd)
{
	int	size;

	size = 0;
	while (cmd[size])
		++size;
	return (size);
}
void	second_son(int fd1[2], int fd_exit, char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	char	**av;
	int		size_cmd;
	
	paths = get_path(envp);
	cmd = get_cmd(argv);
	file_path = try_access(cmd, paths, 1);
	fd_exit = open(argv[4], O_WRONLY);
	dup2(fd1[READ_END], STDIN_FILENO);
	close(fd1[READ_END]);
	dup2(fd_exit, STDOUT_FILENO);
	av = (char **)ft_calloc(sizeof(char *), 3);
	size_cmd = get_size_cmd(cmd);
	if (cmd[1][0] != '-')
		av[0] = cmd[1];
	else
		av[0] = cmd[2];
	if (size_cmd > 2 && cmd[2][0] == '-')
		av[1] = cmd[2];
	else if (size_cmd > 3 && cmd[3][0])
		av[1] = cmd[3];
	execve(file_path, av, NULL);
	close (fd_exit);
	free (av);
}

void	pipex(char *const *argv, char **envp)
{
	int		fd1[2];
	int		fd_exit;
	pid_t	pid;
	int		status;
	
	if (pipe(fd1) < 0)
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
			second_son(fd1, fd_exit, argv, envp);
		else
			close(fd1[READ_END]);
	}
	wait(&status);
	wait(&status);
}

int	main(int argc, char **argv, char **envp)
{
	char *const	*av;
	int			i;

	//if (argc != 5)
		//return (-1);
	pipex(argv, envp);
}
