/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:45:28 by hasivaci          #+#    #+#             */
/*   Updated: 2025/11/26 14:59:40 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

// Belirtilen koordinata renk pikselini yerleştiren fonksiyon
void put_pixel(int x, int y, int color, t_game *game)
{
    if (x >= WIDTH || y >= HEIGHT || x < 0 || y < 0)
        return;
    int index = y * game->size_line + x * game->bbp / 8;
    
    game->data[index] = color & 0xFF;
    game->data[index + 1] = (color >> 8) & 0xFF;
    game->data[index + 2] = (color >> 16) & 0xFF; 
}

// Belirlenen konumda ve boyutta kare çizen fonksiyon (sadece çerçeve)
void draw_square(int x, int y, int size, int color, t_game *game)
{
    for (int i = 0; i < size; i++)
        put_pixel(x + i, y, color, game);
    for (int i = 0; i < size; i++)
        put_pixel(x, y + i, color, game);
    for (int i = 0; i < size; i++)
        put_pixel(x + size, y + i, color, game);
    for (int i = 0; i < size; i++)
        put_pixel(x + i, y + size, color, game);
    
}

// Harita verilerini okuyarak duvarları çizen fonksiyon
void draw_map(t_game *game)
{
    char **map = game->map;
    int color = 0x0000FF;
    for (int y = 0; map[y]; y++)
        for (int x = 0; map[y][x]; x++)
            if (map[y][x] == '1')
                draw_square(x * 64, y * 64, 64, color, game);
            
}

// Tüm ekranı siyah renge boyayan (temizleyen) fonksiyon
void clear_image(t_game *game)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            put_pixel(x, y, 0, game);
}

// Test amaçlı sabit harita verisi döndüren fonksiyon
char **get_map(void)
{
    char **map = malloc(sizeof(char *) * 11);
    map[0] = "111111111111111";
    map[1] = "100000000000001";
    map[2] = "100000000000001";
    map[3] = "100000100000001";
    map[4] = "100000000000001";
    map[5] = "100000010000001";
    map[6] = "100001000000001";
    map[7] = "100000000000001";
    map[8] = "100000000000001";
    map[9] = "111111111111111";
    map[10] = NULL;
    return (map);
}

// Oyun yapılarını ve MLX kütüphanesini başlatan fonksiyon
void init_game(t_game *game)
{
    init_player(&game->player);
    game->mlx = mlx_init();
    game->map = get_map();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Game");
    game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->data = mlx_get_data_addr(game->img, &game->bbp, &game->size_line, &game->endian);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}

// Ana oyun döngüsü - her frame'de çalışan fonksiyon
int draw_loop(t_game *game)
{
    t_player *player = &game->player;
    move_player(player);
    clear_image(game);
    if (DEBUG)
    {
        draw_square(player->x, player->y, 15, 0x00FF00, game);
        draw_map(game);
    }
    
    perform_raycasting(game);
    
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0,0);
    return(0);
}

// Oyunu kapatıp tüm kaynakları temizleyen fonksiyon
int close_game(t_game *game)
{
    // Önce image'ı yok et
    if (game->img && game->mlx)
        mlx_destroy_image(game->mlx, game->img);
    
    // Sonra window'u yok et
    if (game->win && game->mlx)
        mlx_destroy_window(game->mlx, game->win);
    
    // Son olarak display'i yok et (Linux için)
    if (game->mlx)
        mlx_destroy_display(game->mlx);
    
    // MLX pointer'ını free et
    if (game->mlx)
        free(game->mlx);
    
    // Map memory'sini temizle (eğer malloc ile oluşturulduysa)
    if (game->map)
        free(game->map);
    
    exit(0);
}

int main(void)
{
    t_game game;
    
    init_game(&game);

    game.player.game = &game;
    mlx_hook(game.win, 2, 1L<<0, key_press, &game.player);
    mlx_hook(game.win, 3, 1L<<1, key_release, &game.player);
    mlx_hook(game.win, 17, 0, close_game, &game);
    
    mlx_loop_hook(game.mlx, draw_loop, &game);
    // draw_square(WIDTH / 2, HEIGHT / 2, 10, 0x00FF00,&game);
    mlx_loop(game.mlx); 

    return(0);
}

// Belirtilen koordinatın duvara değip değmediğini kontrol eden fonksiyon
// bool touch (float px, float py, t_game *game)
// {
//     int x = px / BLOCK;
//     int y = py / BLOCK;
    
//     if (game->map[y][x] == '1')
//         return(true);
//     return(false);
// }

// // İki nokta arasındaki Öklid mesafesini hesaplayan fonksiyon
// float distance (float x, float y)
// {
//     return(sqrt(x * x + y * y));    
// }

// // Balık gözü etkisini düzelten mesafe hesaplama fonksiyonu
// float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
// {
//     float delta_x = x2 - x1;
//     float delta_y = y2 - y1;
//     float angle = atan2(delta_y, delta_x) - game->player.angle;
//     float fix_dist = distance(delta_x, delta_y) * cos(angle);
//     return(fix_dist);    
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
//         float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
//         // float dist = distance(ray_x - player->x, ray_y - player->y);
//         float height = (BLOCK / dist) * (WIDTH / 2);
//         int start_y = (HEIGHT - height) / 2;
//         int end = start_y + height;
//         while (start_y < end)
//         {
//             put_pixel(i,start_y, 255, game);
//             start_y++;
//         }
//     }
    
    
// }



// Program başlangıç noktası - oyunu başlatan main fonksiyon


// 8. dakikada kaldık...