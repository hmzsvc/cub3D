/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:17:04 by huozturk          #+#    #+#             */
/*   Updated: 2025/03/19 15:19:11 by huozturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	int		slen;
	int		i;
	char	*str;

	slen = ft_strlen(s1);
	i = 0;
	str = (char *)ft_calloc(slen + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (*(s1 + i) != '\0')
	{
		*(str + i) = *(s1 + i);
		i++;
	}
	*(str + i) = '\0';
	return (str);
}
