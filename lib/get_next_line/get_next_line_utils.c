/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:29:58 by meerkus           #+#    #+#             */
/*   Updated: 2025/03/22 14:28:56 by huozturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"

int	ft_strlen_get(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr_get(char *str, int c)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	return (0);
}

char	*ft_strjoin_get(char *s1, char *s2)
{
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (!s1)
	{
		s1 = ft_calloc(1, sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	tmp = ft_calloc(ft_strlen_get(s1) + ft_strlen_get(s2) + 1, sizeof(char));
	if (!tmp)
		return (NULL);
	while (s1[++i])
		tmp[i] = s1[i];
	while (s2[++j])
		tmp[i + j] = s2[j];
	tmp[i + j] = '\0';
	free(s1);
	return (tmp);
}
