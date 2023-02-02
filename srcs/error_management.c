/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:29:36 by adpachec          #+#    #+#             */
/*   Updated: 2023/02/02 12:35:07 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	error_management(int err)
{
	perror(strerror(err));
	exit(err);
}

void	exit_status(int status)
{
	if (status != 0)
		exit(WEXITSTATUS(status));
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
		error_management(EDOM);
	ptr = (void *) malloc(len);
	if (!ptr)
		error_management(ENOMEM);
	ft_bzero(ptr, len);
	return (ptr);
}