/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmzsvc <hmzsvc@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 00:00:00 by hmzsvc            #+#    #+#             */
/*   Updated: 2026/01/24 00:00:00 by hmzsvc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void init_minimap(t_minimap *minimap)
{
    minimap->pos_x = MINIMAP_X;
    minimap->pos_y = MINIMAP_Y;
    minimap->scale = MINIMAP_SCALE;
}

// DEBUG modundaki draw_square fonksiyonunun minimap versiyonu
static void draw_minimap_square(int x, int y, int size, int color, t_game *game)
{
    int i;
    int screen_x;
    int screen_y;
    
    // Minimap koordinatlarına dönüştür ve ekrana çiz
    screen_x = game->minimap.pos_x + (x / game->minimap.scale);
    screen_y = game->minimap.pos_y + (y / game->minimap.scale);
    size = size / game->minimap.scale;
    
    i = -1;
    while (++i < size)
        put_pixel(screen_x + i, screen_y, color, game);
    i = -1;
    
    while (++i < size)
        put_pixel(screen_x, screen_y + i, color, game);
    i = -1;
    
    while (++i < size)
        put_pixel(screen_x + size, screen_y + i, color, game);
    i = -1;
    
    while (++i < size)
        put_pixel(screen_x + i, screen_y + size, color, game);
}

// Minimap kenar çerçevesi
static void draw_minimap_border(t_game *game)
{
    int max_width;
    int max_height;
    int i;
    int j;
    
    // Harita boyutlarını hesapla
    max_width = 0;
    max_height = 0;
    while (game->map[max_height])
    {
        i = 0;
        while (game->map[max_height][i])
            i++;
        if (i > max_width)
            max_width = i;
        max_height++;
    }
    
    // Minimap boyutları (BLOCK cinsinden harita * küçültme oranı)
    max_width = (max_width * BLOCK) / game->minimap.scale;
    max_height = (max_height * BLOCK) / game->minimap.scale;
    
    // Beyaz çerçeve çiz
    i = -MINIMAP_BORDER;
    while (i < max_height + MINIMAP_BORDER)
    {
        j = -MINIMAP_BORDER;
        while (j < max_width + MINIMAP_BORDER)
        {
            // Sadece kenarları çiz
            if (i < 0 || i >= max_height || j < 0 || j >= max_width)
                put_pixel(game->minimap.pos_x + j, 
                         game->minimap.pos_y + i, 
                         0xFFFFFF, game);
            j++;
        }
        i++;
    }
}

// DEBUG modundaki draw_map fonksiyonunun birebir kopyası (minimap için)
static void draw_minimap_map(t_game *game)
{
    char **map;
    int color;
    int x;
    int y;

    y = 0;
    x = 0;
    map = game->map;
    color = 0x0000FF;  // Mavi duvarlar (DEBUG ile aynı)
    
    while (map[y])
    {
        x = 0;
        while (map[y][x])
        {
            if (map[y][x] == '1')
                draw_minimap_square(x * 64, y * 64, 64, color, game);
            x++;
        }
        y++;
    }
}

// DEBUG modundaki player çiziminin minimap versiyonu
static void draw_minimap_player(t_game *game)
{
    t_player *player;
    int player_x;
    int player_y;
    int size;
    int i;
    int j;
    
    player = &game->player;
    
    // Oyuncu pozisyonunu minimap koordinatlarına dönüştür
    player_x = game->minimap.pos_x + (player->x / game->minimap.scale);
    player_y = game->minimap.pos_y + (player->y / game->minimap.scale);
    size = 15 / game->minimap.scale;  // DEBUG'daki 15 pixel boyut
    
    // Yeşil kare çiz (DEBUG ile aynı renk)
    i = 0;
    while (i < size)
    {
        j = 0;
        while (j < size)
        {
            put_pixel(player_x + j, player_y + i, 0x00FF00, game);
            j++;
        }
        i++;
    }
}

// Ana minimap çizim fonksiyonu
void draw_minimap(t_game *game)
{
    draw_minimap_border(game);
    draw_minimap_map(game);
    draw_minimap_player(game);
}