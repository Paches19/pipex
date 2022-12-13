/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/13 17:00:56 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	first_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		fd_input;
	int		err;

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
	close(fd1[WRITE_END]);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);
	if (err != 0)
		error_management();
}

static void	second_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		fd_exit;
	int		err;

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
	if (err != 0)
		error_management();
}

static void	pipex(char *const *argv, char **envp)
{
	int		fd1[2];
	pid_t	pid;
	int		status;

	if (pipe(fd1))
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
	if (status != 0)
		exit(WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		write(2, "ERROR: not enough arguments\n", 29);
		write(2, "ARG: file_input command1 command2 file_output\n", 46);
		exit(1);
	}
	pipex(argv, envp);
	return (0);
}
