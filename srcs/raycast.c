/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/05 17:39:16 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

/* Işın verisi yapısı - tek bir raycasting işlemi için gerekli bilgiler */
typedef struct s_ray
{
    float	angle;      /* Işının açısı (radyan cinsinden) */
    int		map_x;      /* Şu anki grid X pozisyonu */
    int		map_y;      /* Şu anki grid Y pozisyonu */
    float	side_x;     /* Sonraki dikey grid çizgisine mesafe */
    float	side_y;     /* Sonraki yatay grid çizgisine mesafe */
    float	delta_x;    /* Dikey grid çizgileri arası mesafe */
    float	delta_y;    /* Yatay grid çizgileri arası mesafe */
    int		step_x;     /* X yönünde ilerleme (-1 veya +1) */
    int		step_y;     /* Y yönünde ilerleme (-1 veya +1) */
    int		side;       /* Çarpılan duvar yüzü (0=dikey, 1=yatay) */
    float	wall_dist;  /* Duvara dik mesafe */
    float	wall_x;     /* Duvardaki tam çarpma noktası (0.0-1.0 arası) */
}	t_ray;

/* Işın yönünü ve DDA algoritması parametrelerini başlatır */
static void	init_ray_direction(t_ray *ray, t_game *g)
{
    float	dir_x;
    float	dir_y;

    /* Açıdan ışın yön vektörünü hesapla */
    dir_x = cos(ray->angle);
    dir_y = sin(ray->angle);
    
    /* Oyuncu piksel pozisyonunu grid koordinatlarına çevir */
    ray->map_x = (int)(g->player.x / BLOCK);
    ray->map_y = (int)(g->player.y / BLOCK);
    
    /* Sonraki grid çizgisine kadar artış miktarını hesapla */
    ray->delta_x = fabs(1.0 / dir_x);
    ray->delta_y = fabs(1.0 / dir_y);
    
    /* X yönü adımını ve başlangıç yan mesafesini ayarla */
    if (dir_x < 0)
    {
        ray->step_x = -1;  /* Işın sola gidiyor */
        ray->side_x = (g->player.x / BLOCK - ray->map_x) * ray->delta_x;
    }
    else
    {
        ray->step_x = 1;  /* Işın sağa gidiyor */
        ray->side_x = (ray->map_x + 1.0 - g->player.x / BLOCK) * ray->delta_x;
    }
    
    /* Y yönü adımını ve başlangıç yan mesafesini ayarla */
    if (dir_y < 0)
    {
        ray->step_y = -1;  /* Işın yukarı gidiyor */
        ray->side_y = (g->player.y / BLOCK - ray->map_y) * ray->delta_y;
    }
    else
    {
        ray->step_y = 1;  /* Işın aşağı gidiyor */
        ray->side_y = (ray->map_y + 1.0 - g->player.y / BLOCK) * ray->delta_y;
    }
}

/* Verilen grid pozisyonunda duvar olup olmadığını kontrol eder */
static bool	is_wall(int x, int y, t_game *g)
{
    int	max_y;
    int	max_x;

    /* Sınır kontrolü: negatif koordinatlar duvardır */
    if (y < 0 || x < 0)
        return (true);
    
    /* Harita yüksekliğini hesapla (satır sayısı) */
    max_y = 0;
    while (g->map[max_y])
        max_y++;
    
    /* Sınır kontrolü: harita yüksekliğinin ötesi duvardır */
    if (y >= max_y)
        return (true);
    
    /* Mevcut satırın genişliğini hesapla */
    max_x = ft_strlen(g->map[y]);
    
    /* Sınır kontrolü: satır genişliğinin ötesi duvardır */
    if (x >= max_x)
        return (true);
    
    /* Hücrede '1' karakteri var mı kontrol et */
    return (g->map[y][x] == '1');
}

/* DDA algoritması - duvara çarpana kadar grid üzerinde ilerler */
static void	dda_step(t_ray *ray, t_game *g)
{
    /* Işın duvara çarpana kadar döngü */
    while (!is_wall(ray->map_x, ray->map_y, g))
    {
        /* Sonraki grid çizgilerine olan mesafeleri karşılaştır */
        if (ray->side_x < ray->side_y)
        {
            /* Sonraki dikey çizgi daha yakın */
            ray->side_x += ray->delta_x;  /* Sonraki dikey çizgiye geç */
            ray->map_x += ray->step_x;    /* Grid X pozisyonunu güncelle */
            ray->side = 0;                /* Dikey duvara çarptı olarak işaretle */
        }
        else
        {
            /* Sonraki yatay çizgi daha yakın */
            ray->side_y += ray->delta_y;  /* Sonraki yatay çizgiye geç */
            ray->map_y += ray->step_y;    /* Grid Y pozisyonunu güncelle */
            ray->side = 1;                /* Yatay duvara çarptı olarak işaretle */
        }
    }
}

/* Duvara dik mesafeyi hesaplar (balık gözü etkisi düzeltilmiş) */
static void	calc_wall_distance(t_ray *ray, t_game *g)
{
    /* Hangi yüze çarptığına göre mesafeyi hesapla */
    if (ray->side == 0)
        /* Dikey duvar: X mesafesini kullan */
        ray->wall_dist = (ray->map_x - g->player.x / BLOCK
                + (1 - ray->step_x) / 2) / cos(ray->angle);
    else
        /* Yatay duvar: Y mesafesini kullan */
        ray->wall_dist = (ray->map_y - g->player.y / BLOCK
                + (1 - ray->step_y) / 2) / sin(ray->angle);
    
    /* Grid mesafesini piksel mesafesine çevir */
    ray->wall_dist = fabs(ray->wall_dist);
    
    /* Balık gözü düzeltmesi uygula (dik mesafe) */
    ray->wall_dist *= cos(ray->angle - g->player.angle);
}

/* Texture haritalama için duvardaki tam çarpma noktasını hesaplar */
static void	calc_wall_x(t_ray *ray, t_game *g)
{
    /* Duvar oryantasyonuna göre çarpma pozisyonunu hesapla */
    if (ray->side == 0)
        /* Dikey duvar: Y pozisyonunu kullan */
        ray->wall_x = g->player.y / BLOCK + ray->wall_dist
            * sin(ray->angle);
    else
        /* Yatay duvar: X pozisyonunu kullan */
        ray->wall_x = g->player.x / BLOCK + ray->wall_dist
            * cos(ray->angle);
    
    /* Ondalık kısmı al (0.0-1.0 arası texture koordinatı için) */
    ray->wall_x -= floor(ray->wall_x);
}

/* Duvar yönüne göre uygun texture'ı seçer */
static t_texture	*select_texture(t_ray *ray, t_game *g)
{
    /* Dikey duvarlar (Kuzey-Güney ekseni) */
    if (ray->side == 0)
    {
        if (ray->step_x > 0)
            return (&g->e_tex);  /* Işın batıdan geldi, DOĞU texture göster */
        return (&g->w_tex);      /* Işın doğudan geldi, BATI texture göster */
    }
    
    /* Yatay duvarlar (Doğu-Batı ekseni) */
    if (ray->step_y > 0)
        return (&g->s_tex);  /* Işın kuzeyden geldi, GÜNEY texture göster */
    return (&g->n_tex);      /* Işın güneyden geldi, KUZEY texture göster */
}

/* Ekrana tek bir dikey texture sütunu çizer */
static void	draw_wall_column(t_game *g, int x, t_ray *ray, t_texture *tex)
{
    int		h;
    int		start;
    int		end;
    int		y;
    float	step;
    float	tex_pos;
    int		tex_x;
    int		tex_y;

    /* Ekrandaki duvar yüksekliğini hesapla (perspektif projeksiyonu) */
    // h = (int)(BLOCK / ray->wall_dist * (WIDTH / 2));
    h = (int)((WIDTH / 2) / ray->wall_dist);
    
    /* Dikey çizim aralığını hesapla (ekranın ortasında) */
    start = (HEIGHT - h) / 2;
    if (start < 0)
        start = 0;  /* Ekran üstüne sınırla */
    end = start + h;
    if (end > HEIGHT)
        end = HEIGHT;  /* Ekran altına sınırla */
    
    /* Yatay texture koordinatını hesapla */
    tex_x = (int)(ray->wall_x * tex->width);
    if (tex_x < 0)
        tex_x = 0;  /* Negatif indeksi önle */
    if (tex_x >= tex->width)
        tex_x = tex->width - 1;  /* Taşmayı önle */
    
    /* Texture dikey adım boyutunu hesapla */
    step = (float)tex->height / h;
    
    /* Texture Y pozisyonunu başlat (ekran dışı duvarları işle) */
    tex_pos = (start - (HEIGHT - h) / 2) * step;
    if (tex_pos < 0)
        tex_pos = 0;
    
    /* Dikey çizgiyi piksel piksel çiz */
    y = start;
    while (y < end)
    {
        /* Mevcut texture Y koordinatını al */
        tex_y = (int)tex_pos;
        
        /* Texture koordinatı geçerliyse pikseli çiz */
        if (tex_y >= 0 && tex_y < tex->height)
            put_pixel(x, y, get_tex_pixel(tex, tex_x, tex_y), g);
        
        /* Sonraki texture satırına geç */
        tex_pos += step;
        y++;
    }
}

/* Ana render fonksiyonu - tüm ekran genişliği için ışın atar */
void	render_frame(t_game *game)
{
    int			x;
    t_ray		ray;
    t_texture	*tex;

    /* Her ekran sütunu için iterasyon yap (1280 ışın) */
    x = 0;
    while (x < WIDTH)
    {
        /* Mevcut sütun için ışın açısını hesapla (60° FOV) */
        ray.angle = game->player.angle - (PI / 6)
            + ((float)x / WIDTH) * (PI / 3);
            
        /* Adım 1: Işın parametrelerini başlat */
        init_ray_direction(&ray, game);
        
        /* Adım 2: Duvarı bulmak için DDA uygula */
        dda_step(&ray, game);
        
        /* Adım 3: Duvara olan mesafeyi hesapla */
        calc_wall_distance(&ray, game);
        
        /* Adım 4: Texture koordinatını hesapla */
        calc_wall_x(&ray, game);
        
        /* Adım 5: Uygun texture'ı seç */
        tex = select_texture(&ray, game);
        
        /* Adım 6: Duvar sütununu çiz (debug modunda atla) */
        if (!DEBUG)
            draw_wall_column(game, x, &ray, tex);
        x++;
    }
}
