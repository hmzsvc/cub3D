/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:16:59 by huozturk          #+#    #+#             */
/*   Updated: 2025/03/19 15:20:53 by huozturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (s1 != NULL)
		i = ft_strlen(s1);
	if (s2 != NULL)
		j = ft_strlen(s2);
	str = ft_calloc(j + i + 1, sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, i);
	ft_memcpy(str + i, s2, j);
	*(str + i + j) = '\0';
	return (str);
}
