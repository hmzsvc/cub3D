/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:38:26 by hasivaci          #+#    #+#             */
/*   Updated: 2025/11/24 15:23:31 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void init_player(t_player *player)
{
    player->x = WIDTH / 2;
    // player->x = 200;
    player->y = HEIGHT / 2;
    // player->y = 200;
    player->angle = PI / 2;
    
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

void move_player(t_player *player)
{
    int speed = 5;
    float angle_speed = 0.1;
    float cos_angle = cos(player->angle);
    float sin_angle = sin(player->angle);

    if (player->left_rotate)
        player->angle -= angle_speed;
    if (player->right_rotate)
        player->angle += angle_speed;
    if (player->angle > 2 * PI)
        player->angle = 0;
    if (player->angle < 0)
        player->angle = 2 * PI;

    if (player->key_up)
        player->y -= speed;
    if (player->key_down)
        player->y += speed;
    if (player->key_left)
        player->x -= speed;
    if (player->key_right)
        player->x += speed;

    // if (player->key_up)
    // {
    //     player->x += cos_angle * speed;
    //     player->y += sin_angle * speed;
    // }
    // if (player->key_down)
    // {
    //     player->x -= cos_angle * speed;
    //     player->y -= sin_angle * speed;
    // }
    // if (player->key_left)
    // {
    //     player->x += cos_angle * speed;
    //     player->y -= sin_angle * speed;
    // }
    // if (player->key_right)
    // {
    //     player->x -= cos_angle * speed;
    //     player->y += sin_angle * speed;
    // }
}