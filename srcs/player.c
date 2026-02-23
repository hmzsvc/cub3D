/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:38:26 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/23 23:57:22 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

int	key_press(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = true;
	if (keycode == A)
		player->key_left = true;
	if (keycode == S)
		player->key_down = true;
	if (keycode == D)
		player->key_right = true;
	if (keycode == LEFT)
		player->left_rotate = true;
	if (keycode == RIGHT)
		player->right_rotate = true;
	if (keycode == 65307)
		success_close_game(player->game);
	return (0);
}

int	key_release(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;
	return (0);
}

static bool	check_collision(float x, float y, t_game *game)
{
	int	radius;

	radius = COLLISION_RADIUS;
	if (game->map[(int)(y / BLOCK)][(int)(x / BLOCK)] == '1')
		return (true);
	if (game->map[(int)((y - radius) / BLOCK)][(int)((x - radius)
			/ BLOCK)] == '1')
		return (true);
	if (game->map[(int)((y - radius) / BLOCK)][(int)((x + radius)
			/ BLOCK)] == '1')
		return (true);
	if (game->map[(int)((y + radius) / BLOCK)][(int)((x - radius)
			/ BLOCK)] == '1')
		return (true);
	if (game->map[(int)((y + radius) / BLOCK)][(int)((x + radius)
			/ BLOCK)] == '1')
		return (true);
	return (false);
}

static void	rotate_angle(t_player *player, float angle_speed)
{
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle = 0;
	if (player->angle < 0)
		player->angle = 2 * PI;
}

static void	move_up_down(t_player *p, float *nx, float *ny, int speed)
{
	float	cos_a;
	float	sin_a;

	cos_a = cos(p->angle);
	sin_a = sin(p->angle);
	if (p->key_up)
	{
		*nx += cos_a * speed;
		*ny += sin_a * speed;
	}
	if (p->key_down)
	{
		*nx -= cos_a * speed;
		*ny -= sin_a * speed;
	}
}

static void	move_left_right(t_player *p, float *nx, float *ny, int speed)
{
	float	angle;

	if (p->key_left)
	{
		angle = p->angle - PI / 2;
		*nx += cos(angle) * speed;
		*ny += sin(angle) * speed;
	}
	if (p->key_right)
	{
		angle = p->angle + PI / 2;
		*nx += cos(angle) * speed;
		*ny += sin(angle) * speed;
	}
}

void	move_player(t_player *player)
{
	int		speed;
	float	angle_speed;
	float	new_x;
	float	new_y;

	speed = 3;
	angle_speed = 0.03;
	new_x = player->x;
	new_y = player->y;
	rotate_angle(player, angle_speed);
	move_up_down(player, &new_x, &new_y, speed);
	move_left_right(player, &new_x, &new_y, speed);
	if (!check_collision(new_x, new_y, player->game))
	{
		player->x = new_x;
		player->y = new_y;
	}
}
