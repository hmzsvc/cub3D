/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:45:32 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/24 16:15:09 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"
#include "../lib/libft/libft.h"
#include <stdlib.h>

void	extension_control(char *map_argv)
{
	char	*check;

	check = ft_strchr(map_argv, '.');
	if (check)
	{
		if (!ft_strncmp(check, ".cub", ft_strlen(check)))
			return ;
	}
	error_handle("Invalid map extension");
}

int	key_press(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = true;
	if (keycode == A)
		player->key_left = true;
	if (keycode == S)
		player->key_down = true;
	if (keycode == D)
		player->key_right = true;
	if (keycode == LEFT)
		player->left_rotate = true;
	if (keycode == RIGHT)
		player->right_rotate = true;
	if (keycode == 65307)
		success_close_game(player->game);
	return (0);
}

int	key_release(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;
	return (0);
}

void	error_handle(char *err_str)
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
