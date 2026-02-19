/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:58:50 by hasivaci          #+#    #+#             */
/*   Updated: 2024/11/08 19:16:11 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*hmz;

	hmz = (unsigned char *)s;
	while (n > 0)
	{
		if (*hmz == (unsigned char)c)
			return ((void *)hmz);
		n--;
		hmz++;
	}
	return (NULL);
}
