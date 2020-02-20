/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   press_down.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:51:02 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:16:10 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	find_fire(t_wolf *wolf, int x, int y)
{
	wolf->map[y][x] = 0;
	wolf->view_len += 1;
}

void	delete_int_arr(t_wolf *wolf)
{
	int	**iter;
	int i;

	iter = wolf->map;
	i = -1;
	while (++i < wolf->height)
		free(iter[i]);
	free(wolf->map);
	wolf->map = NULL;
}

int		check_number_in_map(t_wolf *wolf, int x, int y)
{
	if (wolf->map[y][x] == 0)
		return (1);
	if (wolf->map[y][x] == 9)
	{
		wolf->bl_you_win = 1;
		return (1);
	}
	if (wolf->map[y][x] == 8)
	{
		find_fire(wolf, x, y);
		return (1);
	}
	if (wolf->map[y][x] == 10)
	{
		delete_int_arr(wolf);
		parser(wolf, "./maps/level2.map");
		print_int_arr(wolf);
		wolf->pl.pov = 0;
		wolf->bl_pause = !wolf->bl_pause;
		start_position(wolf);
		return (1);
	}
	return (0);
}

void	press_up(t_wolf *wolf)
{
	double	dx;
	double	dy;
	int		x;
	int		y;

	dx = STEP * cos(wolf->pl.pov * M_PI / 180);
	dy = STEP * sin(wolf->pl.pov * M_PI / 180);
	x = (dx * 10 + wolf->pl.x_pl) / SQUARE;
	y = (dy * 10 + wolf->pl.y_pl) / SQUARE;
	if (check_number_in_map(wolf, x, y))
	{
		wolf->pl.x_pl += dx;
		wolf->pl.y_pl += dy;
	}
	press_enter(wolf);
}

void	press_down(t_wolf *wolf)
{
	double	dx;
	double	dy;
	int		x;
	int		y;

	dx = STEP * cos(wolf->pl.pov * M_PI / 180);
	dy = STEP * sin(wolf->pl.pov * M_PI / 180);
	x = (wolf->pl.x_pl - dx * 10) / SQUARE;
	y = (wolf->pl.y_pl - dy * 10) / SQUARE;
	if (check_number_in_map(wolf, x, y))
	{
		wolf->pl.x_pl -= dx;
		wolf->pl.y_pl -= dy;
	}
	press_enter(wolf);
}
