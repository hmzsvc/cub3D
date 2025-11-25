/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:42:26 by hasivaci          #+#    #+#             */
/*   Updated: 2025/11/25 15:20:16 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

// Belirtilen koordinatın duvara değip değmediğini kontrol eden fonksiyon
bool touch (float px, float py, t_game *game)
{
    int x = px / BLOCK;
    int y = py / BLOCK;
    
    if (game->map[y][x] == '1')
        return(true);
    return(false);
}

// İki nokta arasındaki Öklid mesafesini hesaplayan fonksiyon
float distance (float x, float y)
{
    return(sqrt(x * x + y * y));    
}

// Balık gözü etkisini düzelten mesafe hesaplama fonksiyonu
float fixed_dist(float x1, float y1, float x2, float y2, t_game *game)
{
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float angle = atan2(delta_y, delta_x) - game->player.angle;
    float fix_dist = distance(delta_x, delta_y) * cos(angle);
    return(fix_dist);    
}

// Raycasting için ışın çizen ve duvar yüksekliği hesaplayan fonksiyon
void draw_line(t_player *player, t_game *game, float start_x, int i)
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x = player->x;
    float ray_y = player->y;
    
    while (!touch(ray_x, ray_y, game))
    {
        if(DEBUG)
            put_pixel(ray_x, ray_y,0xFF0000, game);
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
    if (!DEBUG)
    {
        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        float height = (BLOCK / dist) * (WIDTH / 2);
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
        while (start_y < end)
        {
            put_pixel(i,start_y, 255, game);
            start_y++;
        }
    }
}

// Ana raycasting algoritmasını çalıştıran fonksiyon
void perform_raycasting(t_game *game)
{
    t_player *player = &game->player;
    float fraction = PI / 3 / WIDTH;
    float start_x = player->angle - PI / 6;
    int i = 0;
    
    while (i < WIDTH)
    {
        draw_line(player, game, start_x, i);
        start_x += fraction;
        i++;
    }
}
