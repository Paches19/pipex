/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adpachec <adpachec@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 12:27:43 by adpachec          #+#    #+#             */
/*   Updated: 2023/02/21 11:26:20 by adpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**res;

	if (!s)
		error_management(ENOMEM);
	words = ft_words(s, c);
	res = (char **) ft_calloc(sizeof(char *), (words + 1));
	if (!res)
		error_management(ENOMEM);
	ft_init_matrix(s, c, res, words);
	if (!res)
		error_management(ENOMEM);
	return (res);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		error_management(ENOMEM);
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
	free(s1);
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	len_s1;

	if (!s1 || !s2)
		error_management(ENOMEM);
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
