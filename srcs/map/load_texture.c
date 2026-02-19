/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 15:23:24 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/16 23:45:55 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <stdio.h>

static int	load_tex(t_texture	*tex, char *path)
{
	t_game	*game;

	game = global_game();
	

	tex->width = 64;
	tex->height = 64;	
	
	//printf("GAME_MLX: $%p$ - PATH: $%s$ - width: $%d$ - height: $%d$\n", game->mlx, path, tex->width, tex->height);
	tex->img = mlx_xpm_file_to_image(game->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
		return (0);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);


	//printf("TEX_ADDR: %p - BPP:%d - LINE:%d - END:%d\n", 
	//	(void *)tex->addr, tex->bpp, tex->line_len, tex->endian);
		
	return (1);
}
void	load_all_tex()
{
	t_game	*game;
	
	game = global_game();
	game->n_path = whitespaces_term(game->n_path); // NULL CHECK EKLENECEK
	game->s_path = whitespaces_term(game->s_path); // NULL CHECK EKLENECEK
	game->e_path = whitespaces_term(game->e_path); // NULL CHECK EKLENECEK
	game->w_path = whitespaces_term(game->w_path); // NULL CHECK EKLENECEK
	if (!game->n_path || !game->s_path || !game->e_path || !game->w_path)
	{
		printf("Texture path not found\n");
		exit(1);
	}
	if (!load_tex(&game->n_tex, game->n_path))
		exit (1);
	if (!load_tex(&game->s_tex, game->s_path))
		exit (1);
	if (!load_tex(&game->e_tex, game->e_path))
		exit (1);
	if (!load_tex(&game->w_tex, game->w_path))
		exit (1);
}
int	get_tex_pixel(t_texture *tex, int x, int y)
{
    char	*pixel;
    int		color;

    // Sınır kontrolü
    if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return (0x000000);  // Siyah döndür
    
    // Pixel adresini hesapla
    pixel = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
    
    // Rengi oku (little endian)
    color = *(unsigned int *)pixel;
    
    return (color);
}

// int	get_tex_pixel(t_texture	*tex, int x, int y)
// {
// 	char	*pixel;
// 	int		color;

// 	if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
// 		return (0);
// 	//pixel = tex->addr + ()
// }
