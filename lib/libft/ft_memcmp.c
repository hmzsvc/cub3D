/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:59:08 by hasivaci          #+#    #+#             */
/*   Updated: 2024/11/11 14:01:42 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			h;
	unsigned char	*str1;
	unsigned char	*str2;

	h = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (h < n)
	{
		if (*(str1 + h) != *(str2 + h))
			return (*(str1 + h) - *(str2 + h));
		h++;
	}
	return (0);
}
