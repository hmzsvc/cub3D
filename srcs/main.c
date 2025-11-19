/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:45:28 by hasivaci          #+#    #+#             */
/*   Updated: 2025/11/19 16:37:15 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void init_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, 1280, 720, "Game");
    game->img = mlx_new_image(game->mlx, 1280, 720);
}


int main(void)
{
    t_game game;
    
    init_game(&game);

    mlx_loop(game.mlx);

    return(0);
}