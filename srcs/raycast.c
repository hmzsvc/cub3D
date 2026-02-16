// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   raycast.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
// /*   Updated: 2026/02/09 15:50:57 by hasivaci         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */


#include "../inc/game.h"

static void calc_x(float dir_x, t_ray *ray, t_game *g)
{
    if (dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_x = (g->player.x / BLOCK - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		// printf("buraya girdi\n");
		ray->side_x = (ray->map_x + 1.0 - g->player.x / BLOCK) * ray->delta_x;// en yakın ızgaraya taşıayacak olan çapraz mesafe 
		// printf("side_x == %f\n", ray->side_x);

	}
}

static void	init_ray_direction(t_ray *ray, t_game *g, float dir_x, float dir_y)
{
	ray->map_x = (int)(g->player.x / BLOCK); // player konumu grip hesabı çevir 
	ray->map_y = (int)(g->player.y / BLOCK);
	ray->delta_x = fabs(1.0 / dir_x); // bir sonraki x çizgisine olan uzaklığı birim cinsinden 
	ray->delta_y = fabs(1.0 / dir_y); // bir sonraki y çizgisine olan uzaklığı birim cinsinden varsayım
	// plaeyın adım boyu 
	// printf("map_x bu == %d\n", ray->map_x); 
	// printf("delta_x bu == %f\n", ray->delta_x);
	// printf("player_x bu == %f\n", g->player.x );
	// printf("dir_x bu == %f\n", dir_x);
	
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

	if (y < 0 || x < 0) // üst köşe ve sol köşe 
		return (true);
	max_y = 0;
	while (g->map[max_y]) // max_y yi hüsodaki değer alınabilr ??
		max_y++;
	if (y >= max_y)
		return (true);
	max_x = ft_strlen(g->map[y]); // map_x de aynı şekilde 
	if (x >= max_x)
		return (true);
	return (g->map[y][x] == '1');
}

static void	dda_step(t_ray *ray, t_game *g)
{
	while (!is_wall(ray->map_x, ray->map_y, g)) //hangisi daha yakınsa o duvarın tipi ve gittiği yöne doğru ekleme
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

static void	calc_wall_distance(t_ray *ray, t_game *g) //p ve wall arasındaki net mesafe
{
	if (ray->side == 0)
		ray->wall_dist = (ray->map_x - g->player.x / BLOCK + (1 - ray->step_x/*burası sağ mı yoksa sol mu hesabı yapar*/)
				/ 2) / cos(ray->angle); // hipoyu bulmak için  komşu / cos(açı)
	else
		ray->wall_dist = (ray->map_y - g->player.y / BLOCK + (1 - ray->step_y)
				/ 2) / sin(ray->angle);
	ray->wall_dist = fabs(ray->wall_dist);
	// ray->wall_dist *= cos(ray->angle - g->player.angle);
}

static void	calc_wall_x(t_ray *ray, t_game *g) // duvarın yüzde kaçında hesabı
{
	if (ray->side == 0)
		ray->wall_x = g->player.y / BLOCK + ray->wall_dist * sin(ray->angle);
	else
		ray->wall_x = g->player.x / BLOCK + ray->wall_dist * cos(ray->angle);
	ray->wall_x -= floor(ray->wall_x); // duvarın yüzde kaçında hesabı
}

static t_texture	*select_texture(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&g->e_tex);
		return (&g->w_tex);
	}
	if (ray->step_y > 0)
		return (&g->s_tex);
	return (&g->n_tex);
}
static void calc_draw_data(t_game *g, t_ray *ray, t_texture *tex, t_draw *draw)
{
    float corrected_dist;

    /* 1. Balık gözü düzeltmesini BURADA yap (Geçici değişkene at) */
    corrected_dist = ray->wall_dist * cos(ray->angle - g->player.angle); // mesafeyi ray ve playerın açısı ile çıkart 

    /* 2. Yüksekliği bu DÜZELTİLMİŞ mesafeye göre hesapla */
    draw->h = (int)((WIDTH / 2) / corrected_dist);
    
    draw->start = (HEIGHT - draw->h) / 2;
    if (draw->start < 0)
        draw->start = 0;
    draw->end = draw->start + draw->h;
    if (draw->end > HEIGHT)
        draw->end = HEIGHT;

    /* 3. Texture X hesabı için ray->wall_dist (HAM MESAFE) kullanılıyor */
    /* Önemli: draw->tex_x hesabı buraya gelmeden önce ray->wall_x içinde yapılmıştı */
    /* ray->wall_dist değişmediği için texture artık kaymayacak. */
    draw->tex_x = (int)(ray->wall_x * tex->width);
    if (draw->tex_x < 0)
        draw->tex_x = 0;
    if (draw->tex_x >= tex->width)
        draw->tex_x = tex->width - 1;

    /* 4. Step hesabında yine draw->h kullanıyoruz (sorun yok) */
    draw->step = (float)tex->height / draw->h;
    draw->tex_pos = (draw->start - (HEIGHT - draw->h) / 2) * draw->step;
    if (draw->tex_pos < 0)
        draw->tex_pos = 0;
}


static void	ft_put_draw(t_draw *d, t_game *g, t_texture *tex, int x)
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


void	render_frame(t_game *game)
{
	int			x;
	t_ray		ray;
	t_texture	*tex;
	t_draw		draw;
	float		dir_x;
	float		dir_y;
	// printf("player_angle == %f\n", game->player.angle);
	x = 0;
	while (x < WIDTH)
	{
		ray.angle = game->player.angle - (PI / 6) + ((float)x / WIDTH) * (PI
				/ 3);
		// printf("ray_angle == %f\n", ray.angle);

		dir_x = cos(ray.angle);
		dir_y = sin(ray.angle);
		init_ray_direction(&ray, game, dir_x, dir_y);
		dda_step(&ray, game);
		calc_wall_distance(&ray, game);
		calc_wall_x(&ray, game);
		tex = select_texture(&ray, game);
		calc_draw_data(game, &ray, tex, &draw);
		ft_put_draw(&draw, game, tex, x);
		x++;
		// if (x == 2)
		// {
		// 	exit(1);
		// }
		
	}
	// printf("delta_x bu == %f ray_angle bu == %f\n", ray.delta_x, ray.angle);
	// printf("delta_y bu == %f ray_angle bu == %f\n", ray.delta_y, ray.angle);
	// printf("side_x == %f\n", ray.side_x);
}
