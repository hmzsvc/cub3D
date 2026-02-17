/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:05:14 by hasivaci          #+#    #+#             */
/*   Updated: 2024/11/12 15:20:24 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	h;

	h = 0;
	if (dstsize > 0)
	{
		while (h < dstsize - 1 && *(src + h) != '\0')
		{
			*(dst + h) = *(src + h);
			h++;
		}
		*(dst + h) = '\0';
	}
	return (ft_strlen(src));
}
