/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_halloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 17:52:33 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/15 17:57:33 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "../inc/game.h"

void	*ft_halloc(size_t count, size_t size)
{
	void	*memory;

	memory = halloc(count * size);
	if (memory == NULL)
	{
		return (NULL);
	}
	ft_memset(memory, 0, size * count);
	return (memory);
}