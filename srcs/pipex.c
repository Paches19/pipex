/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2023/02/21 10:44:15 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	first_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_input;

	fd_input = open(argv[1], O_RDONLY);
	if (fd_input < 0)
		error_management(fd_input);
	paths = get_path(envp);
	cmd = ft_split(argv[2], ' ');
	file_path = try_access(cmd, paths);
	dup2(fd_input, STDIN_FILENO);
	close(fd_input);
	close(fd1[READ_END]);
	dup2(fd1[WRITE_END], STDOUT_FILENO);
	err = execve(file_path, cmd, NULL);
	close(fd1[WRITE_END]);
	ft_free_matrix(paths);
	ft_free_matrix(cmd);
	free(file_path);	
	if (err < 0)
		error_management(err);
}

void	second_son(int fd1[2], char *const *argv, char **envp)
{
	char	**paths;
	char	**cmd;
	char	*file_path;
	int		err;
	int		fd_exit;

	fd_exit = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_exit < 0)
		error_management(fd_exit);
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
		error_management(err);
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
		error_management(err);
	pid = fork();
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
	exit_status(status);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		error_management(EINVAL);
	pipex(argv, envp);
	return (0);
}
