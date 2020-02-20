/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:42:32 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:35:04 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	init_image_in_squere_file_xpm(void *mlx, t_image *img,
		int size_squere, char *path)
{
	img->wid = size_squere;
	img->hei = size_squere;
	img->bits_adr = 4;
	img->size_adr = size_squere;
	img->endian = 0;
	if (!(img->img_ptr = mlx_xpm_file_to_image(mlx,
					path, &img->wid, &img->hei)))
		sys_err("Error image back.\n");
	img->data_adr = mlx_get_data_addr(img->img_ptr,
			&img->bits_adr, &img->size_adr, &img->endian);
}

void	init_image_in_file_xpm(void *mlx, t_image *img, char *path)
{
	img->wid = WIDTH;
	img->hei = HEIGHT;
	img->bits_adr = 4;
	img->size_adr = WIDTH;
	img->endian = 0;
	if (!(img->img_ptr = mlx_xpm_file_to_image(mlx,
					path, &img->wid, &img->hei)))
		sys_err("Error image back.\n");
	img->data_adr = mlx_get_data_addr(img->img_ptr,
			&img->bits_adr, &img->size_adr, &img->endian);
}

void	init_image(void *mlx, t_image *img, int width, int height)
{
	img->wid = width;
	img->hei = height;
	img->bits_adr = 4;
	img->size_adr = width;
	img->endian = 0;
	img->img_ptr = mlx_new_image(mlx, width, height);
	img->data_adr = mlx_get_data_addr(img->img_ptr,
			&img->bits_adr, &img->size_adr, &img->endian);
}

void	init_image_walls_texture(t_wolf *wolf)
{
	int		i;
	char	*name_wall;
	char	*number;

	i = -1;
	name_wall = NULL;
	number = NULL;
	while (++i < COUNT_WALL)
	{
		number = ft_itoa(i);
		name_wall = ft_multi_strdup(3, WALLS, number, ".xpm");
		init_image_in_squere_file_xpm(wolf->mlx, &wolf->walls[i],
				SQUARE, name_wall);
		ft_strdel(&number);
		ft_strdel(&name_wall);
	}
}

void	init(t_wolf *wolf)
{
	wolf->pl.x_pl = 160;
	wolf->pl.y_pl = 160;
	wolf->pl.pov = 0;
	wolf->side = 1;
	wolf->bl_pause = 1;
	wolf->bl_you_win = 0;
	wolf->bl_game_over = 0;
	wolf->bl_compass = 0;
	wolf->pl.fov = 60.0;
	wolf->cos_arr = NULL;
	wolf->delta_wid = 0.0;
	wolf->view_len = 3.0;
	wolf->half_hei = HEIGHT / 2;
	wolf->for_squre = SQUARE * 928;
	wolf->mlx = mlx_init();
	wolf->window = mlx_new_window(wolf->mlx, WIDTH, HEIGHT, "Wolf3D");
	init_image(wolf->mlx, &wolf->img, WIDTH, HEIGHT);
	init_image_walls_texture(wolf);
	init_image_in_file_xpm(wolf->mlx, &wolf->img_start, "./texture/Start.xpm");
	init_image_in_file_xpm(wolf->mlx, &wolf->img_game_over,
			"./texture/Game_over.xpm");
	init_image_in_file_xpm(wolf->mlx, &wolf->img_you_win,
			"./texture/you_win.xpm");
}
