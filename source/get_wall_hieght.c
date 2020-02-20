/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall_hieght.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:46:47 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:35:05 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	direction_gor(t_wolf *wolf, t_ray *gor, int *tmp, double angle)
{
	if ((angle > 0 && angle < 180) ||
			(angle < -180 && angle > -360) || angle > 360)
	{
		*tmp = SQUARE;
		gor->y_wall = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE + SQUARE;
		gor->number_wall = 4;
	}
	else
	{
		*tmp = (-1) * SQUARE;
		gor->y_wall = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE;
		gor->number_wall = 3;
	}
}

t_ray	get_len_ray_gor(t_wolf *wolf, double k, double angle)
{
	t_ray	gor;
	int		tmp;

	direction_gor(wolf, &gor, &tmp, angle);
	while (1)
	{
		gor.x_wall = (gor.y_wall - wolf->pl.y_pl) / k + wolf->pl.x_pl;
		if (check_wall_gor(wolf, &gor))
			break ;
		gor.y_wall += tmp;
	}
	gor.ver = 0;
	gor.distance = sqrt(pow(gor.x_wall - wolf->pl.x_pl, 2) +
			pow(gor.y_wall - wolf->pl.y_pl, 2));
	return (gor);
}

void	direction_ver(t_wolf *wolf, t_ray *ver, int *tmp, double angle)
{
	if ((angle > -90 && angle < 90) || (angle > 270) || (angle < -270))
	{
		*tmp = SQUARE;
		ver->x_wall = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE + SQUARE;
		ver->number_wall = 1;
	}
	else
	{
		*tmp = (-1) * SQUARE;
		ver->x_wall = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE;
		ver->number_wall = 2;
	}
}

t_ray	get_len_ray_ver(t_wolf *wolf, double k, double angle)
{
	t_ray	ver;
	int		tmp;

	direction_ver(wolf, &ver, &tmp, angle);
	while (1)
	{
		ver.y_wall = wolf->pl.y_pl + k * (ver.x_wall - wolf->pl.x_pl);
		if (check_wall_vert(wolf, &ver))
			break ;
		ver.x_wall += tmp;
	}
	ver.ver = 1;
	ver.distance = sqrt(pow(ver.x_wall - wolf->pl.x_pl, 2) +
			pow(ver.y_wall - wolf->pl.y_pl, 2));
	return (ver);
}

t_ray	get_wall_height(t_wolf *wolf, double angle)
{
	double	k;
	double	b;
	t_ray	r_v;
	t_ray	r_g;

	k = tan(angle * M_PI / 180);
	b = wolf->pl.y_pl - k * wolf->pl.x_pl;
	if (k == 0)
		return (get_len_ray_ver(wolf, k, angle));
	r_v = get_len_ray_ver(wolf, k, angle);
	r_g = get_len_ray_gor(wolf, k, angle);
	return ((r_v.distance < r_g.distance) ? r_v : r_g);
}
