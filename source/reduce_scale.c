/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reduce_scale.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:52:29 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:18:16 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

int		move_mouse(int x, int y, t_wolf *wolf)
{
	static int pre_x = 0;

	y = 0;
	if (pre_x - x > 0)
	{
		pre_x = x;
		press_left(wolf);
	}
	else
	{
		pre_x = x;
		press_right(wolf);
	}
	return (0);
}

int		move_camera(int key, t_wolf *wolf)
{
	if (wolf->bl_pause)
		return (0);
	if (key == K_UP)
		press_up(wolf);
	else if (key == K_DOWN)
		press_down(wolf);
	else if (key == K_LEFT)
		press_left(wolf);
	else if (key == K_RIGHT)
		press_right(wolf);
	return (0);
}

int		close_windows(void)
{
	sys_err("Normal exit.\n");
	return (0);
}

void	increase_scale(t_wolf *wolf)
{
	wolf->view_len += 0.1;
}

void	reduce_scale(t_wolf *wolf)
{
	wolf->view_len -= 0.1;
	if (wolf->view_len < 0)
		wolf->bl_game_over = 1;
}
