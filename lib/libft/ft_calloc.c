/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:53 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/16 15:10:14 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*hmz;

	hmz = halloc(count * size);
	if (hmz == NULL)
	{
		return (NULL);
	}
	ft_memset(hmz, 0, size * count);
	return (hmz);
}
