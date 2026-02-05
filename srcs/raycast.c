/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmzsvc <hmzsvc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/01/29 17:02:13 by hmzsvc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/game.h"

typedef struct s_ray
{
    float   angle;
    int     map_x;
    int     map_y;
    float   side_x;
    float   side_y;
    float   delta_x;
    float   delta_y;
    int     step_x;
    int     step_y;
    int     side;
    float   wall_dist;
    float   wall_x;
}   t_ray;

static void init_ray_direction(t_ray *ray, t_game *g)
{
    float   dir_x;
    float   dir_y;

    dir_x = cos(ray->angle);
    dir_y = sin(ray->angle);
    ray->map_x = (int)(g->player.x / BLOCK);
    ray->map_y = (int)(g->player.y / BLOCK);
    ray->delta_x = fabs(1.0 / dir_x);
    ray->delta_y = fabs(1.0 / dir_y);
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

static bool is_wall(int x, int y, t_game *g)
{
    int max_y;
    int max_x;

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

static void dda_step(t_ray *ray, t_game *g)
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

static void calc_wall_distance(t_ray *ray, t_game *g)
{
    if (ray->side == 0)
        ray->wall_dist = (ray->map_x - g->player.x / BLOCK
                + (1 - ray->step_x) / 2) / cos(ray->angle);
    else
        ray->wall_dist = (ray->map_y - g->player.y / BLOCK
                + (1 - ray->step_y) / 2) / sin(ray->angle);
    ray->wall_dist = fabs(ray->wall_dist) * BLOCK;
    ray->wall_dist *= cos(ray->angle - g->player.angle);
}

static void calc_wall_x(t_ray *ray, t_game *g)
{
    if (ray->side == 0)
        ray->wall_x = g->player.y / BLOCK + ray->wall_dist
            * sin(ray->angle) / BLOCK;
    else
        ray->wall_x = g->player.x / BLOCK + ray->wall_dist
            * cos(ray->angle) / BLOCK;
    ray->wall_x -= floor(ray->wall_x);
}

static t_texture    *select_texture(t_ray *ray, t_game *g)
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

static void draw_wall_column(t_game *g, int x, t_ray *ray, t_texture *tex)
{
    int     h;
    int     start;
    int     end;
    int     y;
    float   step;
    float   tex_pos;
    int     tex_x;
    int     tex_y;

    h = (int)(BLOCK / ray->wall_dist * (WIDTH / 2));
    start = (HEIGHT - h) / 2;
    if (start < 0)
        start = 0;
    end = start + h;
    if (end > HEIGHT)
        end = HEIGHT;
    tex_x = (int)(ray->wall_x * tex->width);
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= tex->width)
        tex_x = tex->width - 1;
    step = (float)tex->height / h;
    tex_pos = (start - (HEIGHT - h) / 2) * step;
    if (tex_pos < 0)
        tex_pos = 0;
    y = start;
    while (y < end)
    {
        tex_y = (int)tex_pos;
        if (tex_y >= 0 && tex_y < tex->height)
            put_pixel(x, y, get_tex_pixel(tex, tex_x, tex_y), g);
        tex_pos += step;
        y++;
    }
}

void    render_frame(t_game *game)
{
    int         x;
    t_ray       ray;
    t_texture   *tex;

    x = 0;
    while (x < WIDTH)
    {
        ray.angle = game->player.angle - (PI / 6)
            + ((float)x / WIDTH) * (PI / 3);
        init_ray_direction(&ray, game);
        dda_step(&ray, game);
        calc_wall_distance(&ray, game);
        calc_wall_x(&ray, game);
        tex = select_texture(&ray, game);
        if (!DEBUG)
            draw_wall_column(game, x, &ray, tex);
        x++;
    }
}

// #include "../inc/game.h"

// // Belirtilen koordinatın duvara değip değmediğini kontrol eden fonksiyon
// static bool is_wall_hit(float x, float y, t_game *game)
// {
//     int map_x;
//     int map_y;

//     //koordinatları harita indeksine çevir
//     map_x = (int)(x / BLOCK);
//     map_y = (int)(y / BLOCK);

//     // [ÖNEMLİ] Harita dışına çıkarsak duvar sayalım (Segfault önler)
//     // if (map_x < 0 || map_y < 0 || map_y >= map_height ...) kontrolü eklenebilir.

//     if(game->map[map_y][map_x] == '1')
//         return(true);
//     return(false);
// }
// // bool touch (float px, float py, t_game *game)
// // {
// //     int x = px / BLOCK;
// //     int y = py / BLOCK;
    
// //     if (game->map[y][x] == '1')
// //         return(true);
// //     return(false);
// // }

// // İki nokta arasındaki Öklid mesafesini hesaplayan fonksiyon
// // float distance (float x, float y)
// // {
// //     return(sqrt(x * x + y * y));    
// // }

// // Balık gözü etkisini düzelten mesafe hesaplama fonksiyonu
// // float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
// // {
// //     float delta_x = x2 - x1;
// //     float delta_y = y2 - y1;
// //     float angle = atan2(delta_y, delta_x) - game->player.angle;
// //     float fix_dist = distance(delta_x, delta_y) * cos(angle);
// //     return(fix_dist);    
// // }

// static float cast_ray(t_game *game, float ray_angle)
// {
//     float ray_x = game->player.x;
//     float ray_y = game->player.y;
//     float cos_step = cos(ray_angle);
//     float sin_step = sin(ray_angle);

//     while (!is_wall_hit(ray_x, ray_y, game))
//     {
//         if (DEBUG)
//             put_pixel(ray_x, ray_y, 0xFF0000, game);
//         ray_x += cos_step;
//         ray_y += sin_step;
//     }
//     //öklid mesafesi formulü(sqrt((x2-x1)^2 + (y2-y1)^2))
//     return(sqrt(pow(ray_x - game->player.x, 2) + pow(ray_y -game->player.y, 2)));
// }
// // [YENİ] Adım 2B: Hesaplanan mesafeye göre ekrana sütun çiz (TEXTURE TEST)
// static void draw_column(t_game *game, int x_pos, float dist)
// {
//     int wall_height;
//     int start_y;
//     int end_y;
//     int y;
//     int color;
//     int tex_x;
//     int tex_y;

//     // Duvar yüksekliği hesabı
//     wall_height = (int)((BLOCK / dist) * (WIDTH / 2));
    
//     // Tavan ve zemin sınırlarını belirle
//     start_y = (HEIGHT - wall_height) / 2;
//     if (start_y < 0) start_y = 0;
    
//     end_y = start_y + wall_height;
//     if (end_y >= HEIGHT) end_y = HEIGHT - 1;

//     // TEXTURE TEST: Rastgele texture pixel'lerini kullan
//     y = start_y;
//     while (y < end_y)
//     {
//         // Basit texture mapping (test için)
//         tex_x = (x_pos * 2) % game->n_tex.width;  // X pozisyonuna göre
//         tex_y = ((y - start_y) * game->n_tex.height) / wall_height;  // Y pozisyonuna göre
        
//         // Texture'dan rengi oku
//         color = get_tex_pixel(&game->n_tex, tex_x, tex_y);
        
//         // Eğer texture okuma başarısızsa yeşil kullan
//         if (color == 0)
//             color = 0x00FF00;
        
//         put_pixel(x_pos, y, color, game);
//         y++;
//     }
// }

// // [YENİ] Adım 2B: Hesaplanan mesafeye göre ekrana sütun çiz
// // static void draw_column(t_game *game, int x_pos, float dist)
// // {
// //     int wall_height;
// //     int start_y;
// //     int end_y;
// //     int y;

// //     // Duvar yüksekliği hesabı
// //     wall_height = (int)((BLOCK / dist) * (WIDTH / 2));
    
// //     // Tavan ve zemin sınırlarını belirle
// //     start_y = (HEIGHT - wall_height) / 2;
// //     if (start_y < 0) start_y = 0;
    
// //     end_y = start_y + wall_height;
// //     if (end_y >= HEIGHT) end_y = HEIGHT - 1;

// //     // Sadece duvarı çiz (Tavan ve zemin ayrı fonksiyonlarda boyanabilir)
// //     y = start_y;
// //     while (y < end_y)
// //     {
// //         put_pixel(x_pos, y, 0x00FF00, game); // Beyaz renk
// //         y++;
// //     }
// // }
// // Raycasting için ışın çizen ve duvar yüksekliği hesaplayan fonksiyon
// // void draw_line(t_player *player, t_game *game, float start_x, int i)
// // {
// //     float cos_angle = cos(start_x);
// //     float sin_angle = sin(start_x);
// //     float ray_x = player->x;
// //     float ray_y = player->y;
    
// //     while (!touch(ray_x, ray_y, game))
// //     {
// //         if(DEBUG)
// //             put_pixel(ray_x, ray_y,0xFF0000, game);
// //         ray_x += cos_angle;
// //         ray_y += sin_angle;
// //     }
// //     if (!DEBUG)
// //     {
// //         float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game); // Karakterin olduğu konuma göre wall height ayarlamak için sin & cos
// //         float height = (BLOCK / dist) * (WIDTH / 2); // Sin & cos sonucu dönen değer ile her blockun height'i
// //         int start_y = (HEIGHT - height) / 2; // Hesaplanan block yüksekliğinin en alt başlangıç kısmı
// //         int end = start_y + height; // Hesaplanan block yüksekliğinin top noktası
// // 		//printf("START_Y: %d --- HEİGHT: %f --- END: %d \n", start_y, height, end);
// //         while (start_y < end) // En alttan başlayarak her satıra pixel basma
// //         {
// //             put_pixel(i,start_y, 255, game);
// //             start_y++;
// //         }
// //     }
// // }

// // Ana raycasting algoritmasını çalıştıran fonksiyon
// // [YENİ] Adım 3: Ana Döngü
// void render_frame(t_game *game)
// {
//     int     i;
//     float   ray_angle;
//     float   dist;
//     float   angle_step;

//     i = 0;
//     angle_step = (PI / 3) / WIDTH; // Her piksel için açı değişimi
//     ray_angle = game->player.angle - (PI / 6); // Başlangıç açısı

//     while (i < WIDTH)
//     {
//         // 1. Işını at ve mesafeyi al
//         dist = cast_ray(game, ray_angle);
        
//         // 2. Balık gözü (Fisheye) efektini düzelt (Eski fixed_dist buraya geldi)
//         if (!DEBUG)
//         {
//             dist = dist * cos(ray_angle - game->player.angle);
//             // 3. Ekrana çiz
//             draw_column(game, i, dist);
//         }
        
//         ray_angle += angle_step;
//         i++;
//     }
// }
// // void perform_raycasting(t_game *game)
// // {
// //     t_player *player = &game->player;
// //     float fraction = PI / 3 / WIDTH;
// //     float start_x = player->angle - PI / 6;
// //     int i = 0;
    
// //     while (i < WIDTH)
// //     {
// //         draw_line(player, game, start_x, i);
// //         start_x += fraction;
// //         i++;
// //     }
// // }
