/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 17:19:17 by adpachec          #+#    #+#             */
/*   Updated: 2022/12/13 17:03:35 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define READ_END 0
# define WRITE_END 1

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include "errno.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		get_size_cmd(char **cmd);
void	error_management(void);
void	ft_free_matrix(char **matrix);
void	ft_init_matrix(const char *s, char c, char **res, size_t words);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_words(const char *s, char c);
char	**get_path(char **envp);
char	*try_access(char **cmd, char **paths);
char	**get_av(char **cmd);
char	*get_paths_cmd_son_2(char ***paths, char ***cmd, char *const *argv, \
char **envp);

#endif