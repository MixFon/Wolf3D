/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_wall.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:44:39 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:07:21 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

/*
** height_line высота столба линии вертикальной текстуры
** distance расстояние до этой текстуры.
*/

void	print_floor(t_wolf *wolf, int x,
		long double height_line, double distance)
{
	long double angle;
	long double	g;
	double		path;
	t_point		point;
	int			j;

	path = (HEIGHT - height_line) / 2.0;
	if (path < 0)
		return ;
	angle = atan((distance * 2.0) / SQUARE);
	j = HEIGHT - path;
	point.x = x;
	while (++j < HEIGHT)
	{
		point.color = 0x373737;
		g = path / sin(angle);
		point.color = add_shadow(wolf, point.color, g);
		point.y = j;
		put_pixel_adr(wolf, point);
		path--;
	}
}

int		press_key(int key, t_wolf *wolf)
{
	if (key == K_ESC)
		sys_err("Normal exit.\n");
	else if (key == K_ENTER)
		wolf->bl_pause = !wolf->bl_pause;
	else if (wolf->bl_pause)
		return (0);
	else if (key == K_Z)
		increase_scale(wolf);
	else if (key == K_X)
		reduce_scale(wolf);
	else if (key == K_C)
		wolf->bl_compass = !wolf->bl_compass;
	else
		return (0);
	press_enter(wolf);
	return (0);
}

void	drow_vertical_line(t_wolf *wolf, int x, t_ray *ray, double h)
{
	double	j;
	int		num;
	int		len;
	t_point point;

	num = -1;
	j = wolf->half_hei - (ray->distance / 2.0);
	if (j < 0)
	{
		num = (ray->distance - HEIGHT) / 2;
		j = 0;
	}
	point.x = x;
	len = wolf->half_hei + (ray->distance / 2.0);
	while (++j < len)
	{
		if (j > HEIGHT)
			break ;
		point.color = get_color_point(ray,
				wolf->walls[ray->number_wall].data_adr, ++num);
		point.color = add_shadow(wolf, point.color, h);
		point.y = j;
		put_pixel_adr(wolf, point);
	}
	print_floor(wolf, x, ray->distance, h);
}

int		check_wall_vert(t_wolf *wolf, t_ray *ray)
{
	int x;
	int y;

	x = ray->x_wall / SQUARE;
	y = ray->y_wall / SQUARE;
	if (x < 0 || y < 0 || y > wolf->height - 1 || x > wolf->width - 1)
		return (1);
	if (wolf->map[y][x] != 0)
	{
		if (!wolf->bl_compass)
			ray->number_wall = wolf->map[y][x];
		return (1);
	}
	if (wolf->map[y][x - 1] != 0)
	{
		if (!wolf->bl_compass)
			ray->number_wall = wolf->map[y][x - 1];
		return (1);
	}
	return (0);
}

int		check_wall_gor(t_wolf *wolf, t_ray *ray)
{
	int x;
	int y;

	x = ray->x_wall / SQUARE;
	y = ray->y_wall / SQUARE;
	if (x < 0 || y < 0 || y > wolf->height - 1 || x > wolf->width - 1)
		return (1);
	if (wolf->map[y][x] != 0)
	{
		if (!wolf->bl_compass)
			ray->number_wall = wolf->map[y][x];
		return (1);
	}
	if (wolf->map[y - 1][x] != 0)
	{
		if (!wolf->bl_compass)
			ray->number_wall = wolf->map[y - 1][x];
		return (1);
	}
	return (0);
}
