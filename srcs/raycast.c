/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/01/07 01:10:05 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

//// Belirtilen koordinatın duvara değip değmediğini kontrol eden fonksiyon
//bool touch (float px, float py, t_game *game)
//{
//    int x = px / BLOCK;
//    int y = py / BLOCK;
    
//    if (game->map[y][x] == '1')
//        return(true);
//    return(false);
//}

bool	touch(float px, float py, t_game *game)
{
    int	x;
    int	y;
    int	max_y;
    int	max_x;

    x = px / BLOCK;
    y = py / BLOCK;
    if (y < 0 || x < 0)
        return (true);
    max_y = 0;
    while (game->map[max_y])
        max_y++;
    if (y >= max_y)
        return (true);
    max_x = ft_strlen(game->map[y]);
    if (x >= max_x)
        return (true);
    if (game->map[y][x] == '1')
        return (true);
    return (false);
// Belirtilen koordinatın duvara değip değmediğini kontrol eden fonksiyon
static bool is_wall_hit(float x, float y, t_game *game)
{
    int map_x;
    int map_y;

    //koordinatları harita indeksine çevir
    map_x = (int)(x / BLOCK);
    map_y = (int)(y / BLOCK);

    // [ÖNEMLİ] Harita dışına çıkarsak duvar sayalım (Segfault önler)
    // if (map_x < 0 || map_y < 0 || map_y >= map_height ...) kontrolü eklenebilir.

    if(game->map[map_y][map_x] == '1')
        return(true);
    return(false);
}
// bool touch (float px, float py, t_game *game)
// {
//     int x = px / BLOCK;
//     int y = py / BLOCK;
    
//     if (game->map[y][x] == '1')
//         return(true);
//     return(false);
// }

// İki nokta arasındaki Öklid mesafesini hesaplayan fonksiyon
// float distance (float x, float y)
// {
//     return(sqrt(x * x + y * y));    
// }

// Balık gözü etkisini düzelten mesafe hesaplama fonksiyonu
// float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
// {
//     float delta_x = x2 - x1;
//     float delta_y = y2 - y1;
//     float angle = atan2(delta_y, delta_x) - game->player.angle;
//     float fix_dist = distance(delta_x, delta_y) * cos(angle);
//     return(fix_dist);    
// }

int	get_wall_dir(float cos_a, float sin_a)
{
    if (cos_a > 0 && cos_a > sin_a && cos_a > -sin_a)
        return (0);
    if (cos_a < 0 && -cos_a > sin_a && -cos_a > -sin_a)
        return (1);
    if (sin_a > 0)
        return (2);
    return (3);
}

t_texture	*select_tex(t_game *game, int direction)
{
    if (direction == 0)
        return (&game->e_tex);
    if (direction == 1)
        return (&game->w_tex);
    if (direction == 2)
        return (&game->s_tex);
    return (&game->n_tex);
}


//int	calc_tex_x(float ray_x, float ray_y, t_texture *tex, int dir)
//{
//    float	wall_hit;
//    int		tex_x;

//    if (dir == 0 || dir == 1)
//        wall_hit = ray_y;
//    else
//        wall_hit = ray_x;
//    wall_hit = wall_hit - (int)wall_hit;
//    tex_x = (int)(wall_hit * tex->width);
//    if (tex_x < 0)
//        tex_x = 0;
//    if (tex_x >= tex->width)
//        tex_x = tex->width - 1;
//    return (tex_x);
//}

int	calc_tex_x(float ray_x, float ray_y, t_texture *tex, int dir)
{
    float	wall_hit;
    int		tex_x;

    if (dir == 0 || dir == 1)
        wall_hit = ray_y / BLOCK;  // ✅ BLOCK'a böl
    else
        wall_hit = ray_x / BLOCK;
    
    wall_hit = wall_hit - (int)wall_hit;  // Kesirli kısmı al (0.0 - 1.0)
    tex_x = (int)(wall_hit * tex->width);
    
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= tex->width)
        tex_x = tex->width - 1;
    
    return (tex_x);
}

static void	draw_wall_pixel(t_game *game, t_texture *tex, int i, int *vals)
{
    int		tex_y;
    int		color;
    float	tex_pos;

    tex_pos = vals[3];
    while (vals[2] < vals[1])
    {
        tex_y = (int)tex_pos;
        tex_pos = tex_pos + ((float)tex->height / vals[0]);
        color = get_tex_pixel(tex, vals[4], tex_y);
        put_pixel(i, vals[2], color, game);
        vals[2]++;
    }
}

void	draw_tex_wall(t_game *g, t_texture *tex, int i, float dist, int tx)
{
    float	height;
    int		start_y;
    int		end_y;
    int		draw_start;
    int		draw_end;
    float	tex_step;
    float	tex_pos;
    int		tex_y;
    int		color;
    int		y;

    if (dist < 1.0)
        dist = 1.0;
    height = (BLOCK / dist) * (WIDTH / 2);
    start_y = (HEIGHT - (int)height) / 2;
    end_y = start_y + (int)height;
    draw_start = start_y;
    draw_end = end_y;
    if (draw_start < 0)
        draw_start = 0;
    if (draw_end > HEIGHT)
        draw_end = HEIGHT;
    tex_step = (float)tex->height / height;
    tex_pos = (draw_start - start_y) * tex_step;
    y = draw_start;
    while (y < draw_end)
    {
        tex_y = (int)tex_pos;
        if (tex_y >= 0 && tex_y < tex->height)
        {
            color = get_tex_pixel(tex, tx, tex_y);
            put_pixel(i, y, color, g);
        }
        tex_pos = tex_pos + tex_step;
static float cast_ray(t_game *game, float ray_angle)
{
    float ray_x = game->player.x;
    float ray_y = game->player.y;
    float cos_step = cos(ray_angle);
    float sin_step = sin(ray_angle);

    while (!is_wall_hit(ray_x, ray_y, game))
    {
        if (DEBUG)
            put_pixel(ray_x, ray_y, 0xFF0000, game);
        ray_x += cos_step;
        ray_y += sin_step;
    }
    //öklid mesafesi formulü(sqrt((x2-x1)^2 + (y2-y1)^2))
    return(sqrt(pow(ray_x - game->player.x, 2) + pow(ray_y -game->player.y, 2)));
}
// [YENİ] Adım 2B: Hesaplanan mesafeye göre ekrana sütun çiz (TEXTURE TEST)
static void draw_column(t_game *game, int x_pos, float dist)
{
    int wall_height;
    int start_y;
    int end_y;
    int y;
    int color;
    int tex_x;
    int tex_y;

    // Duvar yüksekliği hesabı
    wall_height = (int)((BLOCK / dist) * (WIDTH / 2));
    
    // Tavan ve zemin sınırlarını belirle
    start_y = (HEIGHT - wall_height) / 2;
    if (start_y < 0) start_y = 0;
    
    end_y = start_y + wall_height;
    if (end_y >= HEIGHT) end_y = HEIGHT - 1;

    // TEXTURE TEST: Rastgele texture pixel'lerini kullan
    y = start_y;
    while (y < end_y)
    {
        // Basit texture mapping (test için)
        tex_x = (x_pos * 2) % game->n_tex.width;  // X pozisyonuna göre
        tex_y = ((y - start_y) * game->n_tex.height) / wall_height;  // Y pozisyonuna göre
        
        // Texture'dan rengi oku
        color = get_tex_pixel(&game->n_tex, tex_x, tex_y);
        
        // Eğer texture okuma başarısızsa yeşil kullan
        if (color == 0)
            color = 0x00FF00;
        
        put_pixel(x_pos, y, color, game);
        y++;
    }
}

//void	draw_tex_wall(t_game *g, t_texture *tex, int i, int *params)
//{
//    int		vals[5];
//    float	height;

//    height = (BLOCK / params[0]) * (WIDTH / 2);
//    vals[0] = (int)height;
//    vals[1] = (HEIGHT - vals[0]) / 2;
//    vals[1] = vals[1] + vals[0];
//    vals[2] = (HEIGHT - vals[0]) / 2;
//    vals[3] = 0;
//    vals[4] = params[1];
//    draw_wall_pixel(g, tex, i, vals);
//}

void	cast_ray(t_player *p, t_game *g, float start_x, float *ray)
{
    ray[0] = cos(start_x);
    ray[1] = sin(start_x);
    ray[2] = p->x;
    ray[3] = p->y;
    while (!touch(ray[2], ray[3], g))
    {
        if (DEBUG)
            put_pixel(ray[2], ray[3], 0xFF0000, g);
        ray[2] = ray[2] + ray[0];
        ray[3] = ray[3] + ray[1];
    }
}

void	draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float		ray[4];
    int			direction;
    t_texture	*tex;
    float		dist;      // ✅ FLOAT
    int			tex_x;

    cast_ray(player, game, start_x, ray);
    dist = fixed_dist(player->x, player->y, ray[2], ray[3], game);
    direction = get_wall_dir(ray[0], ray[1]);
    tex = select_tex(game, direction);
    tex_x = calc_tex_x(ray[2], ray[3], tex, direction);
    draw_tex_wall(game, tex, i, dist, tex_x);
}

void	perform_raycasting(t_game *game)
{
    t_player	*player;
    float		fraction;
    float		start_x;
    int			i;

    player = &game->player;
    fraction = PI / 3 / WIDTH;
    start_x = player->angle - PI / 6;
    i = 0;
    while (i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x = start_x + fraction;
        i++;
    }
}
//// Raycasting için ışın çizen ve duvar yüksekliği hesaplayan fonksiyon
//void draw_line(t_player *player, t_game *game, float start_x, int i)
//{
//    float cos_angle = cos(start_x);
//    float sin_angle = sin(start_x);
//    float ray_x = player->x;
//    float ray_y = player->y;
    
//    while (!touch(ray_x, ray_y, game))
//    {
//        if(DEBUG)
//            put_pixel(ray_x, ray_y,0xFF0000, game);
//        ray_x += cos_angle;
//        ray_y += sin_angle;
//    }
//    if (!DEBUG)
//    {
//        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game); // Karakterin olduğu konuma göre wall height ayarlamak için sin & cos
//        float height = (BLOCK / dist) * (WIDTH / 2); // Sin & cos sonucu dönen değer ile her blockun height'i
//        int start_y = (HEIGHT - height) / 2; // Hesaplanan block yüksekliğinin en alt başlangıç kısmı
//        int end = start_y + height; // Hesaplanan block yüksekliğinin top noktası
//		//printf("START_Y: %d --- HEİGHT: %f --- END: %d \n", start_y, height, end);
//        while (start_y < end) // En alttan başlayarak her satıra pixel basma
//        {
//            put_pixel(i,start_y, 255, game);
//            start_y++;
//        }
//    }
//}

//// Ana raycasting algoritmasını çalıştıran fonksiyon
//void perform_raycasting(t_game *game)
//{
//    t_player *player = &game->player;
//    float fraction = PI / 3 / WIDTH;
//    float start_x = player->angle - PI / 6;
//    int i = 0;
    
//    while (i < WIDTH)
//    {
//        draw_line(player, game, start_x, i);
//        start_x += fraction;
//        i++;
//    }
//}
// [YENİ] Adım 2B: Hesaplanan mesafeye göre ekrana sütun çiz
// static void draw_column(t_game *game, int x_pos, float dist)
// {
//     int wall_height;
//     int start_y;
//     int end_y;
//     int y;

//     // Duvar yüksekliği hesabı
//     wall_height = (int)((BLOCK / dist) * (WIDTH / 2));
    
//     // Tavan ve zemin sınırlarını belirle
//     start_y = (HEIGHT - wall_height) / 2;
//     if (start_y < 0) start_y = 0;
    
//     end_y = start_y + wall_height;
//     if (end_y >= HEIGHT) end_y = HEIGHT - 1;

//     // Sadece duvarı çiz (Tavan ve zemin ayrı fonksiyonlarda boyanabilir)
//     y = start_y;
//     while (y < end_y)
//     {
//         put_pixel(x_pos, y, 0x00FF00, game); // Beyaz renk
//         y++;
//     }
// }
// Raycasting için ışın çizen ve duvar yüksekliği hesaplayan fonksiyon
// void draw_line(t_player *player, t_game *game, float start_x, int i)
// {
//     float cos_angle = cos(start_x);
//     float sin_angle = sin(start_x);
//     float ray_x = player->x;
//     float ray_y = player->y;
    
//     while (!touch(ray_x, ray_y, game))
//     {
//         if(DEBUG)
//             put_pixel(ray_x, ray_y,0xFF0000, game);
//         ray_x += cos_angle;
//         ray_y += sin_angle;
//     }
//     if (!DEBUG)
//     {
//         float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game); // Karakterin olduğu konuma göre wall height ayarlamak için sin & cos
//         float height = (BLOCK / dist) * (WIDTH / 2); // Sin & cos sonucu dönen değer ile her blockun height'i
//         int start_y = (HEIGHT - height) / 2; // Hesaplanan block yüksekliğinin en alt başlangıç kısmı
//         int end = start_y + height; // Hesaplanan block yüksekliğinin top noktası
// 		//printf("START_Y: %d --- HEİGHT: %f --- END: %d \n", start_y, height, end);
//         while (start_y < end) // En alttan başlayarak her satıra pixel basma
//         {
//             put_pixel(i,start_y, 255, game);
//             start_y++;
//         }
//     }
// }

// Ana raycasting algoritmasını çalıştıran fonksiyon
// [YENİ] Adım 3: Ana Döngü
void render_frame(t_game *game)
{
    int     i;
    float   ray_angle;
    float   dist;
    float   angle_step;

    i = 0;
    angle_step = (PI / 3) / WIDTH; // Her piksel için açı değişimi
    ray_angle = game->player.angle - (PI / 6); // Başlangıç açısı

    while (i < WIDTH)
    {
        // 1. Işını at ve mesafeyi al
        dist = cast_ray(game, ray_angle);
        
        // 2. Balık gözü (Fisheye) efektini düzelt (Eski fixed_dist buraya geldi)
        if (!DEBUG)
        {
            dist = dist * cos(ray_angle - game->player.angle);
            // 3. Ekrana çiz
            draw_column(game, i, dist);
        }
        
        ray_angle += angle_step;
        i++;
    }
}
// void perform_raycasting(t_game *game)
// {
//     t_player *player = &game->player;
//     float fraction = PI / 3 / WIDTH;
//     float start_x = player->angle - PI / 6;
//     int i = 0;
    
//     while (i < WIDTH)
//     {
//         draw_line(player, game, start_x, i);
//         start_x += fraction;
//         i++;
//     }
// }
