/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:46:18 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/23 17:14:22 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void	calc_wall_distance(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
		ray->wall_dist = ray->side_x - ray->delta_x;
	else
		ray->wall_dist = ray->side_y - ray->delta_y;
}

void	calc_wall_x(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
		ray->wall_x = g->player.y / BLOCK + ray->wall_dist * sin(ray->angle);
	else
		ray->wall_x = g->player.x / BLOCK + ray->wall_dist * cos(ray->angle);
	ray->wall_x -= floor(ray->wall_x);
}

t_texture	*select_texture(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&g->w_tex);
		return (&g->e_tex);
	}
	if (ray->step_y > 0)
		return (&g->n_tex);
	return (&g->s_tex);
}

void	calc_draw_data(t_game *g, t_ray *ray, t_texture *tex, t_draw *draw)
{
	float	corrected_dist;

	corrected_dist = ray->wall_dist * cos(ray->angle - g->player.angle);
	draw->h = (int)((WIDTH / 2) / corrected_dist);
	draw->start = (HEIGHT - draw->h) / 2;
	if (draw->start < 0)
		draw->start = 0;
	draw->end = draw->start + draw->h;
	if (draw->end > HEIGHT)
		draw->end = HEIGHT;
	draw->tex_x = (int)(ray->wall_x * tex->width);
	if (draw->tex_x < 0)
		draw->tex_x = 0;
	if (draw->tex_x >= tex->width)
		draw->tex_x = tex->width - 1;
	draw->step = (float)tex->height / draw->h;
	draw->tex_pos = (draw->start - (HEIGHT - draw->h) / 2) * draw->step;
	if (draw->tex_pos < 0)
		draw->tex_pos = 0;
}

void	ft_put_draw(t_draw *d, t_game *g, t_texture *tex, int x)
{
	int	y;

	y = 0;
	while (y < d->start)
	{
		put_pixel(x, y, g->ceiling_color, g);
		y++;
	}
	while (y < d->end)
	{
		d->tex_y = (int)d->tex_pos;
		if (d->tex_y >= 0 && d->tex_y < tex->height)
			put_pixel(x, y, get_tex_pixel(tex, d->tex_x, d->tex_y), g);
		d->tex_pos += d->step;
		y++;
	}
	while (y < HEIGHT)
	{
		put_pixel(x, y, g->floor_color, g);
		y++;
	}
}
