/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   press_lift.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:49:37 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:35:02 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	clear_image(t_image *img_ptr)
{
	char	*temp;
	int		i;
	int		len;

	temp = img_ptr->data_adr;
	i = -1;
	len = img_ptr->wid * 4 * img_ptr->hei;
	while (++i < len)
		temp[i] = 0;
}

int		check_pause_or_game_ever(t_wolf *wolf)
{
	if (wolf->bl_you_win)
	{
		mlx_put_image_to_window(wolf->mlx, wolf->window,
				wolf->img_you_win.img_ptr, 0, 0);
		return (1);
	}
	if (wolf->bl_game_over)
	{
		mlx_put_image_to_window(wolf->mlx, wolf->window,
				wolf->img_game_over.img_ptr, 0, 0);
		return (1);
	}
	if (wolf->bl_pause)
	{
		mlx_put_image_to_window(wolf->mlx, wolf->window,
				wolf->img_start.img_ptr, 0, 0);
		return (1);
	}
	return (0);
}

int		press_enter(t_wolf *wolf)
{
	int		i;
	t_ray	ray;
	double	angle;
	double	distance_fish;

	i = -1;
	if (check_pause_or_game_ever(wolf))
		return (0);
	angle = wolf->pl.pov - wolf->pl.fov / 2;
	while (++i < WIDTH)
	{
		ray = get_wall_height(wolf, angle);
		distance_fish = ray.distance;
		ray.distance = ray.distance * wolf->cos_arr[i];
		ray.distance = wolf->for_squre / ray.distance;
		drow_vertical_line(wolf, i, &ray, distance_fish);
		angle += wolf->delta_wid;
	}
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->img.img_ptr, 0, 0);
	clear_image(&wolf->img);
	return (0);
}

void	press_right(t_wolf *wolf)
{
	wolf->pl.pov += ANGLE;
	wolf->pl.pov = wolf->pl.pov % 360;
	press_enter(wolf);
}

void	press_left(t_wolf *wolf)
{
	wolf->pl.pov -= ANGLE;
	wolf->pl.pov = wolf->pl.pov % 360;
	press_enter(wolf);
}
