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

// #include "../inc/game.h"

// /* Işın verisi yapısı - Raycasting işlemi için anlık durum bilgileri */
// typedef struct s_ray
// {
//     float   angle;      /* Işının atıldığı açı (radyan) */
//     int     map_x;      /* Işının haritada bulunduğu anlık X karesi */
//     int     map_y;      /* Işının haritada bulunduğu anlık Y karesi */
//     float   side_x;     /* Bir sonraki X kenarına olan uzaklık */
//     float   side_y;     /* Bir sonraki Y kenarına olan uzaklık */
//     float   delta_x;    /* Bir X karesi gitmek için ışının kat edeceği mesafe */
//     float   delta_y;    /* Bir Y karesi gitmek için ışının kat edeceği mesafe */
//     int     step_x;     /* X ekseninde ilerleme yönü (-1: Sol, +1: Sağ) */
//     int     step_y;     /* Y ekseninde ilerleme yönü (-1: Yukarı, +1: Aşağı) */
//     int     side;       /* Son çarpılan duvar yüzü (0: Dikey, 1: Yatay) */
//     float   wall_dist;  /* Duvara olan dik mesafe (Grid birimi cinsinden) */
//     float   wall_x;     /* Duvar yüzeyindeki tam çarpma noktası (0.0 - 1.0) */
// }   t_ray;

// /* Işın yön vektörlerini ve DDA başlangıç parametrelerini hesaplar */
// static void init_ray_direction(t_ray *ray, t_game *g, float dir_x, float dir_y)
// {
//     /* Oyuncunun piksel konumunu grid (kutu) koordinatına çevir */
//     ray->map_x = (int)(g->player.x / BLOCK);
//     ray->map_y = (int)(g->player.y / BLOCK);
    
//     /* Bir birim kare ilerlemek için gereken delta mesafelerini hesapla */
//     ray->delta_x = fabs(1.0 / dir_x);
//     ray->delta_y = fabs(1.0 / dir_y);
    
//     /* X ekseni için adım yönünü ve ilk kenar mesafesini (side_x) belirle */
//     if (dir_x < 0)
//     {
//         ray->step_x = -1;  /* Sola gidiyor */
//         ray->side_x = (g->player.x / BLOCK - ray->map_x) * ray->delta_x;
//     }
//     else
//     {
//         ray->step_x = 1;  /* Sağa gidiyor */
//         ray->side_x = (ray->map_x + 1.0 - g->player.x / BLOCK) * ray->delta_x;
//     }
    
//     /* Y ekseni için adım yönünü ve ilk kenar mesafesini (side_y) belirle */
//     if (dir_y < 0)
//     {
//         ray->step_y = -1;  /* Yukarı gidiyor */
//         ray->side_y = (g->player.y / BLOCK - ray->map_y) * ray->delta_y;
//     }
//     else
//     {
//         ray->step_y = 1;  /* Aşağı gidiyor */
//         ray->side_y = (ray->map_y + 1.0 - g->player.y / BLOCK) * ray->delta_y;
//     }
// }

// /* Verilen grid koordinatının duvar olup olmadığını kontrol eder */
// static bool is_wall(int x, int y, t_game *g)
// {
//     int max_y;
//     int max_x;

//     /* Harita sınırları dışı kontrolü (Negatif indeks) */
//     if (y < 0 || x < 0)
//         return (true);
    
//     /* Harita satır sayısını hesapla */
//     max_y = 0;
//     while (g->map[max_y])
//         max_y++;
    
//     /* Y ekseni taşma kontrolü */
//     if (y >= max_y)
//         return (true);
    
//     /* İlgili satırın uzunluğunu hesapla */
//     max_x = ft_strlen(g->map[y]);
    
//     /* X ekseni taşma kontrolü */
//     if (x >= max_x)
//         return (true);
    
//     /* Koordinattaki karakter '1' (Duvar) mi? */
//     return (g->map[y][x] == '1');
// }

// /* DDA (Digital Differential Analyzer) algoritmasını çalıştırır */
// static void dda_step(t_ray *ray, t_game *g)
// {
//     /* Duvar bulunana kadar ışını grid üzerinde ilerlet */
//     while (!is_wall(ray->map_x, ray->map_y, g))
//     {
//         /* Hangi eksendeki kenar daha yakınsa o yönde ilerle */
//         if (ray->side_x < ray->side_y)
//         {
//             ray->side_x += ray->delta_x;  /* X kenarına zıpla */
//             ray->map_x += ray->step_x;    /* Grid X'i güncelle */
//             ray->side = 0;                /* Çarpılan yüzey: Dikey */
//         }
//         else
//         {
//             ray->side_y += ray->delta_y;  /* Y kenarına zıpla */
//             ray->map_y += ray->step_y;    /* Grid Y'yi güncelle */
//             ray->side = 1;                /* Çarpılan yüzey: Yatay */
//         }
//     }
// }

// /* Işının kat ettiği mesafeyi hesaplar ve balık gözü etkisini düzeltir */
// static void calc_wall_distance(t_ray *ray, t_game *g)
// {
//     /* Çarpılan yüze göre dik mesafeyi (Euclidean değil) hesapla */
//     /* Not: Sonuç piksel değil, GRID BİRİMİ (kaç kutu ötede) cinsindendir */
//     if (ray->side == 0)
//         ray->wall_dist = (ray->map_x - g->player.x / BLOCK
//                 + (1 - ray->step_x) / 2) / cos(ray->angle);
//     else
//         ray->wall_dist = (ray->map_y - g->player.y / BLOCK
//                 + (1 - ray->step_y) / 2) / sin(ray->angle);
    
//     /* Mesafenin negatif olma ihtimaline karşı mutlak değer al */
//     ray->wall_dist = fabs(ray->wall_dist);
    
//     /* Balık gözü (Fisheye) efektini düzeltmek için açıyı normalize et */
//     ray->wall_dist *= cos(ray->angle - g->player.angle);
// }

// /* Texture kaplaması için duvar yüzeyindeki tam X koordinatını bulur */
// static void calc_wall_x(t_ray *ray, t_game *g)
// {
//     /* Duvarın yönüne göre tam çarpma noktasını hesapla */
//     /* Not: wall_dist grid biriminde olduğu için BLOCK'a bölmeye gerek yoktur */
//     if (ray->side == 0)
//         /* Dikey duvar: Oyuncunun Y konumu + Y yönündeki ilerleme */
//         ray->wall_x = g->player.y / BLOCK + ray->wall_dist
//             * sin(ray->angle);
//     else
//         /* Yatay duvar: Oyuncunun X konumu + X yönündeki ilerleme */
//         ray->wall_x = g->player.x / BLOCK + ray->wall_dist
//             * cos(ray->angle);
    
//     /* Sadece ondalık kısmı al (0.0 ile 1.0 arası) */
//     /* Bu değer texture'ın hangi sütununun çizileceğini belirler */
//     ray->wall_x -= floor(ray->wall_x);
// }

// /* Işının çarptığı duvarın yönüne göre doğru texture'ı seçer */
// static t_texture    *select_texture(t_ray *ray, t_game *g)
// {
//     /* Dikey duvara çarptıysa (Doğu veya Batı) */
//     if (ray->side == 0)
//     {
//         if (ray->step_x > 0)
//             return (&g->e_tex);  /* Işın sağa gidiyorsa DOĞU yüzünü görür */
//         return (&g->w_tex);      /* Işın sola gidiyorsa BATI yüzünü görür */
//     }
    
//     /* Yatay duvara çarptıysa (Kuzey veya Güney) */
//     if (ray->step_y > 0)
//         return (&g->s_tex);  /* Işın aşağı gidiyorsa GÜNEY yüzünü görür */
//     return (&g->n_tex);      /* Işın yukarı gidiyorsa KUZEY yüzünü görür */
// }
// static void ft_put_draw(t_draw *d, t_game *g, t_texture *tex, int x)
// {
//     int     y;
//     // burada tavan basılıyor
//     y = 0;
//     while (y < d->start)
//     {
//         put_pixel(x, y, g->ceiling_color, g);
//         y++;
//     }
//     /* Sütunu yukarıdan aşağıya boya */
//     while (y < d->end)
//     {
//         d->tex_y = (int)d->tex_pos;
        
//         /* Texture sınırları içinde güvenli çizim */
//         if (d->tex_y >= 0 && d->tex_y < tex->height)
//             put_pixel(x, y, get_tex_pixel(tex, d->tex_x, d->tex_y), g);
        
//         d->tex_pos += d->step;
//         y++;
//     }
//     while (y < HEIGHT)
//     {
//         put_pixel(x, y, g->floor_color, g);
//         y++;
//     }
// }
// /* Hesaplanan verilere göre ekrana tek bir dikey sütun çizer */
// static void calc_draw_data(t_game *g, t_ray *ray, t_texture *tex, t_draw *draw)
// {
//     //1. adım /* Duvar yüksekliğini hesapla. (1.0 / dist) formülü grid sistemine uygundur */
//     draw->h = (int)((WIDTH / 2) / ray->wall_dist);
    
//     // 2. adım /* Çizimin başlayacağı ve biteceği Y koordinatlarını belirle */ /* en üst yer tavan 0 en alt nokta pencerenin HEİGHT */
//     draw->start = (HEIGHT - draw->h) / 2;
//     if (draw->start < 0)
//         draw->start = 0;  /* Ekranın üstünden taşmayı engelle */
//     draw->end = draw->start + draw->h;
//     if (draw->end > HEIGHT)
//         draw->end = HEIGHT;  /* Ekranın altından taşmayı engelle */
    
//     // 3. adım  buradan devam et /* Texture üzerindeki X koordinatını piksel cinsinden hesapla */
//     draw->tex_x = (int)(ray->wall_x * tex->width);
//     if (draw->tex_x < 0) 
//         draw->tex_x = 0;
//     if (draw->tex_x >= tex->width) 
//         draw->tex_x = tex->width - 1;
    
//     /* Ekran pikselleri ile texture pikselleri arasındaki adım oranını bul */
//     draw->step = (float)tex->height / draw->h;
    
//     /* Texture başlangıç pozisyonunu ayarla (Ekran dışı kalan kısımları hesaba kat) */
//     draw->tex_pos = (draw->start - (HEIGHT - draw->h) / 2) * draw->step;
//     if (draw->tex_pos < 0)
//         draw->tex_pos = 0;
// }


// /* Ana render döngüsü: Ekran genişliğince ışın atar ve çizim yapar */
// void    render_frame(t_game *game)
// {
//     int         x;
//     t_ray       ray;
//     t_texture   *tex;
//     t_draw  draw;
//     float   dir_x;
//     float   dir_y;

//     x = 0;
//     while (x < WIDTH)
//     {
//         /* Her sütun için ışın açısını hesapla (FOV 60 derece) */
//         ray.angle = game->player.angle - (PI / 6)
//             + ((float)x / WIDTH) * (PI / 3);

//         /* Açının kosinüs ve sinüs bileşenlerini al */
//         dir_x = cos(ray.angle);
//         dir_y = sin(ray.angle);
//         /* Raycasting adımlarını sırayla çalıştır */
//         init_ray_direction(&ray, game, dir_x, dir_y);   /* 1. Başlat */
//         dda_step(&ray, game);             /* 2. DDA ile duvar bul */
//         calc_wall_distance(&ray, game);   /* 3. Mesafeyi hesapla */
//         calc_wall_x(&ray, game);          /* 4. Texture X'i bul */
//         tex = select_texture(&ray, game); /* 5. Texture seç */
//         calc_draw_data(game, &ray, tex, &draw);
        
//         /* Debug modu kapalıysa çizimi yap */
//         if (!DEBUG)
//             ft_put_draw(&draw, game, tex, x);
//         x++;
//     }
// }

#include "../inc/game.h"

typedef struct s_ray
{
	float			angle;
	int				map_x;
	int				map_y;
	float			side_x;
	float			side_y;
	float			delta_x;
	float			delta_y;
	int				step_x;
	int				step_y;
	int				side;
	float			wall_dist;
	float			wall_x;
}					t_ray;

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
		ray->side_x = (ray->map_x + 1.0 - g->player.x / BLOCK) * ray->delta_x;
	}
}

static void	init_ray_direction(t_ray *ray, t_game *g, float dir_x, float dir_y)
{
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

static void	calc_wall_distance(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
		ray->wall_dist = (ray->map_x - g->player.x / BLOCK + (1 - ray->step_x)
				/ 2) / cos(ray->angle);
	else
		ray->wall_dist = (ray->map_y - g->player.y / BLOCK + (1 - ray->step_y)
				/ 2) / sin(ray->angle);
	ray->wall_dist = fabs(ray->wall_dist);
	// ray->wall_dist *= cos(ray->angle - g->player.angle);
}

static void	calc_wall_x(t_ray *ray, t_game *g)
{
	if (ray->side == 0)
		ray->wall_x = g->player.y / BLOCK + ray->wall_dist * sin(ray->angle);
	else
		ray->wall_x = g->player.x / BLOCK + ray->wall_dist * cos(ray->angle);
	ray->wall_x -= floor(ray->wall_x);
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

static void calc_draw_data(t_game *g, t_ray *ray, t_texture *tex, t_draw *draw)
{
    float corrected_dist;

    /* 1. Balık gözü düzeltmesini BURADA yap (Geçici değişkene at) */
    corrected_dist = ray->wall_dist * cos(ray->angle - g->player.angle);

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

void	render_frame(t_game *game)
{
	int			x;
	t_ray		ray;
	t_texture	*tex;
	t_draw		draw;
	float		dir_x;
	float		dir_y;

	x = 0;
	while (x < WIDTH)
	{
		ray.angle = game->player.angle - (PI / 6) + ((float)x / WIDTH) * (PI
				/ 3);
		dir_x = cos(ray.angle);
		dir_y = sin(ray.angle);
		init_ray_direction(&ray, game, dir_x, dir_y);
		dda_step(&ray, game);
		calc_wall_distance(&ray, game);
		calc_wall_x(&ray, game);
		tex = select_texture(&ray, game);
		calc_draw_data(game, &ray, tex, &draw);
		if (!DEBUG)
			ft_put_draw(&draw, game, tex, x);
		x++;
	}
}
