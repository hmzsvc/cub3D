/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:55:03 by hasivaci          #+#    #+#             */
/*   Updated: 2024/11/10 14:56:44 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	h;
	int	sign;
	int	result;

	h = 0;
	sign = 1;
	result = 0;
	while (str[h] == 32 || (str[h] >= 9 && str[h] <= 13))
		h++;
	if (str[h] == '-' || str[h] == '+')
	{
		if (str[h] == '-')
			sign *= -1;
		h++;
	}
	while (str[h] != '\0' && ft_isdigit(str[h]))
	{
		result *= 10;
		result += str[h] - 48;
		h++;
	}
	return (result * sign);
}
