/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 03:17:19 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/23 23:59:04 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"
#include "../lib/libft/libft.h"

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

void	error_handle(char	*err_str)
{
	t_game	*game;
	
	game = global_game();
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(err_str, 2);
	close_game(game);
}

void	put_pixel(int x, int y, int color, t_game *game)
{
	int	index;

	if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
		return ;
	index = y * game->size_line + x * game->bbp / 8;
	game->data[index] = color & 0xFF;
	game->data[index + 1] = (color >> 8) & 0xFF;
	game->data[index + 2] = (color >> 16) & 0xFF;
}