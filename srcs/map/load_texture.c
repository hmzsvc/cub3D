/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:23:24 by hsyn              #+#    #+#             */
/*   Updated: 2025/12/21 19:06:36 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <stdio.h>

static int	load_tex(t_texture	*tex, char *path)
{
	t_game	*game;

	game = global_game();
	

	tex->width = 60;
	tex->height = 60;
	
	
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
		return (0);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);


	printf("\nTEX_ADDR: %s\n", tex->addr);
	return (1);
}
int	load_all_tex()
{
	t_game	*game;
	
	game = global_game();
	//printf("LOAD_GAME_MLX: #%p#  -  LOAD_GAME_IMG: #%p#\n", game->mlx, game->img);
	
	if (!load_tex(&game->n_tex, "../../assets/map/wall-64x64.xpm"))
		return (0);
	if (!load_tex(&game->s_tex, "../../assets/map/wall-64x64.xpm"))
		return (0);
	if (!load_tex(&game->e_tex, "../../assets/map/wall-64x64.xpm"))
		return (0);
	if (!load_tex(&game->w_tex, "../../assets/map/wall-64x64.xpm"))
		return (0);
	return (1);
}

int	get_tex_pixel(t_texture	*tex, int x, int y)
{
	char	*pixel;
	int		color;

	if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
		return (0);
	//pixel = tex->addr + ()
}