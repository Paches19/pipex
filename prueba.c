/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 18:13:26 by adpachec          #+#    #+#             */
/*   Updated: 2022/11/22 18:48:37 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(void)
{
	char	*av[] = {"ls", "-l", NULL};
	char	*env[] = {NULL};
	char 	*av2[] = {"wc", "-l", NULL};
	char	cmd[] = "/bin/ls";
	int		errno;

	errno = execve(cmd, av, env) < 0;
	if (errno)
		perror(strerror(errno));
}
