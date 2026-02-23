/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:23:24 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/23 23:47:55 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int	load_tex(t_texture	*tex, char *path)
{
	t_game	*game;

	game = global_game();
	tex->width = 64;
	tex->height = 64;
	tex->img = mlx_xpm_file_to_image(game->mlx, path,
			&tex->width, &tex->height);
	if (!tex->img)
		error_handle("Texture path not found");
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->line_len, &tex->endian);
	if (!tex->addr)
		error_handle("Addr Error");
	return (1);
}

static void	text_path_check(char *path)
{
	int		fd;
	t_game	*game;

	game = global_game();
	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_handle("Text path error");
	close (fd);
}

void	load_all_tex(void)
{
	t_game	*game;

	game = global_game();
	text_path_check(game->n_path);
	text_path_check(game->s_path);
	text_path_check(game->e_path);
	text_path_check(game->w_path);
	if (!load_tex(&game->n_tex, game->n_path))
		error_handle("Load tex error");
	if (!load_tex(&game->s_tex, game->s_path))
		error_handle("Load tex error");
	if (!load_tex(&game->e_tex, game->e_path))
		error_handle("Load tex error");
	if (!load_tex(&game->w_tex, game->w_path))
		error_handle("Load tex error");
}

int	get_tex_pixel(t_texture *tex, int x, int y)
{
	char	*pixel;
	int		color;

	if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
		return (0x000000);
	pixel = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
	color = *(unsigned int *)pixel;
	return (color);
}
