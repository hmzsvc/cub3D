/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:32:12 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/16 15:10:47 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*hmz;
	int		slen;
	int		i;

	i = 0;
	if (!s || !f)
		return (NULL);
	slen = ft_strlen(s);
	hmz = (char *)ft_calloc(slen + 1, sizeof(char));
	if (!hmz)
		return (NULL);
	while (s[i] != '\0')
	{
		hmz[i] = (*f)(i, s[i]);
		i++;
	}
	*(hmz + i) = '\0';
	return (hmz);
}
