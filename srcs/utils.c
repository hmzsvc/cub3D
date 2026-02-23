/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:17:19 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/20 03:21:00 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void	extension_control(char *map_argv)
{
	char	*check;
	
	check = ft_strchr(map_argv, '.');
	if (check)
	{
		if (!ft_strncmp(check, ".cub", ft_strlen(check)))
			return ;
	}
	printf("Invalid map extension\n");
	exit (1);
}