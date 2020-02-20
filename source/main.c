/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 08:46:55 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:13:05 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	calculate_tan_cos(t_wolf *wolf)
{
	int		i;
	double	delta;

	i = -1;
	delta = (double)wolf->pl.fov / 2;
	wolf->delta_wid = (double)wolf->pl.fov / WIDTH;
	if (!(wolf->cos_arr = (long double*)malloc(sizeof(long double) * WIDTH)))
		sys_err("Error malloc.\n");
	while (++i < WIDTH)
	{
		wolf->cos_arr[i] = cosl(delta * M_PI / 180);
		delta -= wolf->delta_wid;
	}
}

int		check_time(t_wolf *wolf)
{
	static clock_t	start_clock = 0;
	clock_t			cur_clock;

	cur_clock = clock() / CLOCKS_PER_SEC;
	if (cur_clock - start_clock > 1)
	{
		start_clock = cur_clock;
		reduce_scale(wolf);
		press_enter(wolf);
	}
	return (0);
}

void	infill_position(t_wolf *wolf, int i, int j)
{
	wolf->pl.x_pl = SQUARE * j + SQUARE / 2;
	wolf->pl.y_pl = SQUARE * i + SQUARE / 2;
	wolf->map[i][j] = 0;
}

void	start_position(t_wolf *wolf)
{
	int		i;
	int		j;
	int		bl;

	i = -1;
	bl = 0;
	while (++i < wolf->height)
	{
		j = -1;
		while (++j < wolf->width)
		{
			if (wolf->map[i][j] == 0 && bl == 0)
			{
				infill_position(wolf, i, j);
				bl = 1;
			}
			if (wolf->map[i][j] < 0)
			{
				infill_position(wolf, i, j);
				return ;
			}
		}
	}
	if (!bl)
		sys_err("Error map.\n");
}

int		main(int ac, char **av)
{
	t_wolf wolf;

	if (ac != 2)
		sys_err("Two arguments.\n");
	init(&wolf);
	parser(&wolf, av[1]);
	print_int_arr(&wolf);
	start_position(&wolf);
	calculate_tan_cos(&wolf);
	mlx_key_hook(wolf.window, press_key, &wolf);
	mlx_loop_hook(wolf.mlx, check_time, &wolf);
	mlx_put_image_to_window(wolf.mlx, wolf.window,
			wolf.img_start.img_ptr, 0, 0);
	mlx_hook(wolf.window, 2, 0xfffff, move_camera, &wolf);
	mlx_hook(wolf.window, 6, 0xfffff, move_mouse, &wolf);
	mlx_hook(wolf.window, 17, 0xfffff, close_windows, NULL);
	mlx_loop(wolf.mlx);
	return (0);
}
