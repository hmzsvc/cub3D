/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:23:24 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/20 04:12:05 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <stdio.h>

static int	load_tex(t_texture	*tex, char *path)
{
	t_game	*game;

	game = global_game();
	// printf("GEGEGEGEGE\n");
	// if (!path)
	// {
	// 	printf("Texture path not found --%s\n", path);
	// 	close_game(game);
	// 	return (0);
	// }
	tex->width = 64;
	tex->height = 64;
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Texture path not found\n");
		close_game(game);
		return (0);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
	return (1);
}

#include <fcntl.h>
#include <unistd.h>


static void	text_path_check(char *path)
{
	int	fd;
	t_game	*game;

	game = global_game();
	fd = open(path, O_RDONLY);
    if (fd < 0)
	{
		//clear_garbage();
		printf("Text path error\n");
		close_game(game);
        exit (1);
	}
    close(fd);
}

void	load_all_tex()
{
	t_game	*game;
	
	game = global_game();
	
	//game->n_path = whitespaces_term(game->n_path); // NULL CHECK EKLENECEK
	//game->s_path = whitespaces_term(game->s_path); // NULL CHECK EKLENECEK
	//game->e_path = whitespaces_term(game->e_path); // NULL CHECK EKLENECEK
	//game->w_path = whitespaces_term(game->w_path); // NULL CHECK EKLENECEK
	//if (!game->n_path || !game->s_path || !game->e_path || !game->w_path)
	//	exit(1);
	text_path_check(game->n_path); // BURADA HATA VE LEAK VAR
	text_path_check(game->s_path);
	text_path_check(game->e_path);
	text_path_check(game->w_path);

	if (!load_tex(&game->n_tex, game->n_path))
		exit(1);
	if (!load_tex(&game->s_tex, game->s_path))
		exit(1);
	if (!load_tex(&game->e_tex, game->e_path))
		exit(1);
	if (!load_tex(&game->w_tex, game->w_path))
		exit(1);
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
