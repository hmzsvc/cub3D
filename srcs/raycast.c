/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/23 17:15:03 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"
#include <math.h>

static void	calc_x(float dir_x, t_ray *ray, t_game *g)
{
	if (dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_x = (g->player.x / BLOCK - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_x = (ray->map_x + 1.0 - g->player.x / BLOCK) * ray->delta_x;
	}
}

static void	init_ray_direction(t_ray *ray, t_game *g)
{
	float	dir_x;
	float	dir_y;

	dir_x = cos(ray->angle);
	dir_y = sin(ray->angle);
	ray->map_x = (int)(g->player.x / BLOCK);
	ray->map_y = (int)(g->player.y / BLOCK);
	ray->delta_x = fabs(1.0 / dir_x);
	ray->delta_y = fabs(1.0 / dir_y);
	calc_x(dir_x, ray, g);
	if (dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_y = (g->player.y / BLOCK - ray->map_y) * ray->delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_y = (ray->map_y + 1.0 - g->player.y / BLOCK) * ray->delta_y;
	}
}

static bool	is_wall(int x, int y, t_game *g)
{
	int	max_y;
	int	max_x;

	if (y < 0 || x < 0)
		return (true);
	max_y = 0;
	while (g->map[max_y])
		max_y++;
	if (y >= max_y)
		return (true);
	max_x = ft_strlen(g->map[y]);
	if (x >= max_x)
		return (true);
	return (g->map[y][x] == '1');
}

static void	dda_step(t_ray *ray, t_game *g)
{
	while (!is_wall(ray->map_x, ray->map_y, g))
	{
		if (ray->side_x < ray->side_y)
		{
			ray->side_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
	}
}

void	render_frame(t_game *game)
{
	int			x;
	t_ray		ray;
	t_texture	*tex;
	t_draw		draw;

	x = 0;
	while (x < WIDTH)
	{
		ray.angle = game->player.angle - (PI / 6) + ((float)x / WIDTH) * (PI
				/ 3);
		init_ray_direction(&ray, game);
		dda_step(&ray, game);
		calc_wall_distance(&ray, game);
		calc_wall_x(&ray, game);
		tex = select_texture(&ray, game);
		calc_draw_data(game, &ray, tex, &draw);
		ft_put_draw(&draw, game, tex, x);
		x++;
	}
}
