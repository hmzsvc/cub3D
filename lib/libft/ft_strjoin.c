/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:04:58 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/16 15:44:52 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*hmz;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	j = ft_strlen(s2);
	i = ft_strlen(s1);
	hmz = ft_calloc(j + i + 1, sizeof(char));
	if (!hmz)
		return (NULL);
	ft_memcpy(hmz, s1, i);
	ft_memcpy(hmz + i, s2, j);
	hmz[i + j] = '\0';
	return (hmz);
}
