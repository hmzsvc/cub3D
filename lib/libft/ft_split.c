/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 11:41:40 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/16 15:45:02 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	ft_count_words(char const *s, char sep)
{
	size_t	count;

	if (!*s || !s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == sep)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != sep)
				s++;
		}
	}
	return (count);
}

static char	**ft_split_words(char **res, char const *s, char sep, int wordcnt)
{
	char const	*start;
	int			i;

	i = 0;
	while (*s && i < wordcnt)
	{
		while (*s == sep)
			s++;
		start = s;
		while (*s && *s != sep)
			s++;
		res[i] = ft_substr(start, 0, s - start);
		i++;
	}
	res[i] = NULL;
	return (res);
}

char	**ft_split(char const *s, char c)
{
	int		wordcount;
	char	**result;

	if (!s)
		return (NULL);
	wordcount = ft_count_words(s, c);
	result = (char **)ft_calloc(wordcount + 1, sizeof(char *));
	if (!result)
		return (NULL);
	return (ft_split_words(result, s, c, wordcount));
}
