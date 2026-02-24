/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:45:28 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/24 12:13:40 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void	open_window(void)
{
	t_game	*game;

	game = global_game();
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->img)
		error_handle("Image init fail");
	game->data = mlx_get_data_addr(game->img, &game->bbp, &game->size_line,
			&game->endian);
	if (!game->data)
		error_handle("Data init fail");
	game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "cub3D");
	if (!game->win)
		error_handle("Window init fail");
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

int	main(int ac, char **av)
{
	t_game	*game;

	if (ac != 2)
		error_handle("Av error");
	extension_control(av[1]);
	game = global_game();
	init_player(&game->player);
	init_game(game);
	read_map(av[1]);
	if (!game->map)
		error_handle("Map could not be loaded");
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
