/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:38:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/02 22:52:11 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void init_player(t_player *player)
{
    player->x = WIDTH / 2;
    // player->x = 200;
    player->y = HEIGHT / 2;
    // player->y = 200;
    player->angle = 0;
	player->dir_check = 0;
    
    player->key_up = false;
    player->key_down = false;
    player->key_right = false;
    player->key_left = false;
    
    player->left_rotate = false;
    player->right_rotate = false;
}

int key_press(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = true;
    if (keycode == S)
        player->key_down = true;
    if (keycode == A)
        player->key_left = true;
    if (keycode == D)
        player->key_right = true;
    
    if (keycode == LEFT)
        player->left_rotate = true;
    if (keycode == RIGHT)
        player->right_rotate = true;
    if (keycode == 65307)
        close_game(player->game);
    return(0);
}

int key_release(int keycode, t_player *player)
{
    if (keycode == W)
        player->key_up = false;
    if (keycode == S)
        player->key_down = false;
    if (keycode == A)
        player->key_left = false;
    if (keycode == D)
        player->key_right = false;
    if (keycode == LEFT)
        player->left_rotate = false;
    if (keycode == RIGHT)
        player->right_rotate = false;
    return(0);
}

// Verilen pozisyonun duvarla çarpışıp çarpışmadığını kontrol eder
static bool check_collision(float x, float y, t_game *game)
{
    int radius = COLLISION_RADIUS;
    
    // Karakterin 4 köşesini ve merkezi kontrol et
    // Merkez
    if (game->map[(int)(y / BLOCK)][(int)(x / BLOCK)] == '1')
        return (true);
    
    // Sol üst köşe
    if (game->map[(int)((y - radius) / BLOCK)][(int)((x - radius) / BLOCK)] == '1')
        return (true);
    
    // Sağ üst köşe
    if (game->map[(int)((y - radius) / BLOCK)][(int)((x + radius) / BLOCK)] == '1')
        return (true);
    
    // Sol alt köşe
    if (game->map[(int)((y + radius) / BLOCK)][(int)((x - radius) / BLOCK)] == '1')
        return (true);
    
    // Sağ alt köşe
    if (game->map[(int)((y + radius) / BLOCK)][(int)((x + radius) / BLOCK)] == '1')
        return (true);
    
    return (false);
}

void move_player(t_player *player)
{
    int speed = 3;
    float angle_speed = 0.03;
    float cos_angle = cos(player->angle);
    float sin_angle = sin(player->angle);
    
    // YENİ POZİSYONLARI GEÇİCİ DEĞİŞKENLERDE SAKLA
    float new_x = player->x;
    float new_y = player->y;

    // AÇIYI DÖNDÜR (Duvar kontrolüne gerek yok)
    if (player->left_rotate)
        player->angle -= angle_speed;
    if (player->right_rotate)
        player->angle += angle_speed;
    if (player->angle > 2 * PI)
        player->angle = 0;
    if (player->angle < 0)
        player->angle = 2 * PI;

    // İLERİ (W)
    if (player->key_up)
    {
        new_x += cos_angle * speed;
        new_y += sin_angle * speed;
    }
    
    // GERİ (S)
    if (player->key_down)
    {
        new_x -= cos_angle * speed;
        new_y -= sin_angle * speed;
    }
    
    // SOLA STRAFE (A)
    if (player->key_left)
    {
        new_x += cos(player->angle - PI / 2) * speed;
        new_y += sin(player->angle - PI / 2) * speed;
    }
    
    // SAĞA STRAFE (D)
    if (player->key_right)
    {
        new_x += cos(player->angle + PI / 2) * speed;
        new_y += sin(player->angle + PI / 2) * speed;
    }

     if (!check_collision(new_x, new_y, player->game))
    {
        player->x = new_x;
        player->y = new_y;
    }
    
    // // 🚨 DUVAR KONTROLÜ - YENİ POZİSYON DUVARIN İÇİNDE Mİ?
    // int map_x = (int)(new_x / BLOCK);
    // int map_y = (int)(new_y / BLOCK);
    
    // // Eğer yeni pozisyon duvar değilse ('1' değilse), harekete izin ver
    // if (player->game->map[map_y][map_x] != '1')
    // {
    //     player->x = new_x;
    //     player->y = new_y;
    // }
    // // Eğer duvarsa ('1' ise), hareket etme - eski konumda kal
}

// void move_player(t_player *player)
// {
//     int speed = 3;
//     float angle_speed = 0.03;
//     float cos_angle = cos(player->angle);
//     float sin_angle = sin(player->angle);

//     if (player->left_rotate)
//         player->angle -= angle_speed;
//     if (player->right_rotate)
//         player->angle += angle_speed;
//     if (player->angle > 2 * PI)
//         player->angle = 0;
//     if (player->angle < 0)
//         player->angle = 2 * PI;

//     // if (player->key_up)
//     //     player->y -= speed;
//     // if (player->key_down)
//     //     player->y += speed;
//     // if (player->key_left)
//     //     player->x -= speed;
//     // if (player->key_right)
//     //     player->x += speed;

//     if (player->key_up)
//     {
//         player->x += cos_angle * speed;
//         player->y += sin_angle * speed;
//     }
//     if (player->key_down)
//     {
//         player->x -= cos_angle * speed;
//         player->y -= sin_angle * speed;
//     }
//     // if (player->key_left)
//     // {
//     //     player->x -= cos_angle * speed;
//     //     player->y -= sin_angle * speed;
//     // }
//     // if (player->key_right)
//     // {
//     //     player->x += cos_angle * speed;
//     //     player->y += sin_angle * speed;
//     // }
//     // SOLA STRAFE (A) - Baktığın yönün SOL tarafına git (angle - 90°)
//     if (player->key_left)
//     {
//         player->x += cos(player->angle - PI / 2) * speed;
//         player->y += sin(player->angle - PI / 2) * speed;
//     }
    
//     // SAĞA STRAFE (D) - Baktığın yönün SAĞ tarafına git (angle + 90°)
//     if (player->key_right)
//     {
//         player->x += cos(player->angle + PI / 2) * speed;
//         player->y += sin(player->angle + PI / 2) * speed;
//     }
// }
