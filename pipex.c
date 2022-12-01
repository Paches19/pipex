/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:19:47 by adpachec          #+#    #+#             */
/*   Updated: 2022/11/22 19:04:54 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_management(void)
{
	printf("ERROR\n");
	exit (-1);
}

void	pipex(char *const *argv, char **envp)
{
	int		fd1[2];
	int		fd_exit;
	pid_t	pid;
	int		status;
	char	*av1[] = {"ls", "-l", NULL};
	char	*av2[] = {"ls", "-l", NULL};
	
	if (pipe(fd1) < 0)
		error_management();
	pid = fork();
	if (pid < 0)
		error_management();
	if (!pid)
	{
		close(fd1[READ_END]);
		dup2(fd1[WRITE_END], STDOUT_FILENO);
		close(fd1[WRITE_END]);
		execve("/bin/ls", av1, NULL);
	}
	else
	{
		close(fd1[WRITE_END]);
		pid = fork();
		if (!pid)
		{
			fd_exit = open(argv[4], O_WRONLY);
			dup2(fd1[READ_END], STDIN_FILENO);
			close(fd1[READ_END]);
			dup2(fd_exit, STDOUT_FILENO);
			execve("/bin/wc", av2, NULL);
		}
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

	if (argc != 5)
		return (-1);
	i = -1;
	while (envp[++i])
		printf("env[%d]: %s\n\n", i, envp[i]);
	//pipex(av, envp);
}
