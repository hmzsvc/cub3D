/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:45:28 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/22 16:03:26 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

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

void	open_window()
{
	t_game	*game;

	game = global_game();

	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
	{
		printf("Image init fail\n");
		close_game(game);
	}
	game->data = mlx_get_data_addr(game->img, &game->bbp, &game->size_line,
			&game->endian);
	if (!game->data)
	{
		printf("Data init fail\n");
		close_game(game);
	}
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
	{
		printf("Window init fail\n");
		close_game(game);
	}
}

void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		close_game(game);
	}
	
	game->map_element_count = 0;
	game->map_lines_count = 0;
	game->map_flag = 0;
	game->newline_flag = 0;
	game->gap_check = 0;
}

int	draw_loop(t_game *game)
{
	t_player	*player;

	player = &game->player;
	move_player(player);
	render_frame(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}
int	success_close_game(t_game *game)
{
	if (game->n_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->n_tex.img);
	if (game->s_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->s_tex.img);
	if (game->e_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->e_tex.img);
	if (game->w_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->w_tex.img);
	if (game->img && game->mlx)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	clear_garbage();
	exit(0);
}


void	close_game(t_game *game)
{
	if (game->n_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->n_tex.img);
	if (game->s_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->s_tex.img);
	if (game->e_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->e_tex.img);
	if (game->w_tex.img && game->mlx)
		mlx_destroy_image(game->mlx, game->w_tex.img);
	if (game->img && game->mlx)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win && game->mlx)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	clear_garbage();
	exit(1);
}

t_game	*global_game(void)
{
	static t_game	*game;

	if (!game)
		game = (t_game *)ft_calloc(1, sizeof(t_game));
	if (!game)
	{
		exit(1);
	}
	return (game);
}

int	main(int ac, char **av)
{
	t_game	*game;

	if (ac != 2)
	{
		printf("Av error\n");
		exit (1);
	}
	extension_control(av[1]);
	game = global_game();
	init_player(&game->player);
	init_game(game);
	read_map(av[1]);
	if (!game->map)
	{
		printf("Map could not be loaded\n");
		close_game(game);
	}
	load_all_tex();
	open_window();
	game->player.game = game;
	mlx_hook(game->win, 2, 1L << 0, key_press, &game->player);
	mlx_hook(game->win, 3, 1L << 1, key_release, &game->player);
	mlx_hook(game->win, 17, 0, success_close_game, game);
	mlx_loop_hook(game->mlx, draw_loop, game);
	mlx_loop(game->mlx);
	return (0);
}
