/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 08:46:55 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 13:41:16 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	init_image_in_squere_file_xpm(void *mlx, t_image *img,
		int size_squere , char *path)
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

void	init_image(void *mlx, t_image *img, int width , int height)
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
	while (++i < COUNT_WALLS)
	{
		number = ft_itoa(i);
		name_wall = ft_multi_strdup(3, WALLS, number, ".xpm"); 
		init_image_in_squere_file_xpm(wolf->mlx, &wolf->walls[i],
				SQUARE, name_wall);
		ft_printf("number = {%s}, name_wall = [%s]\n", number, name_wall);
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

void	put_pixel_adr(t_wolf *wolf, t_point point)
{
	int i;

	if (point.x >= WIDTH || point.y >= HEIGHT || point.x <= 0 || point.y <= 0)
		return ;
	i = (point.x * wolf->img.bits_adr / 8) + (point.y * wolf->img.size_adr);
	wolf->img.data_adr[i] = point.color >> 24;
	wolf->img.data_adr[++i] = point.color >> 16;
	wolf->img.data_adr[++i] = point.color >> 8;
	wolf->img.data_adr[++i] = 0;
}

void	infill_int_map(int *iarr, char **carr)
{
	int		i;
	char	**tmp;

	i = -1;
	tmp = carr;
	while (*carr != NULL)
	{
		iarr[++i] = ft_atoi(*carr);
		ft_strdel(carr);
		carr++;
	}
	free(tmp);
}

void	print_int_arr(t_wolf *wolf)
{
	int i;
	int j;

	i = -1;
	while (++i < wolf->height)
	{
		j = -1;
		while (++j < wolf->width)
			ft_printf("%d ", wolf->map[i][j]);
		ft_putchar('\n');
	}
}

int		get_color_point(t_ray *ray, char *data_adr, int num_pix, double H)
{
	int		color;
	int		x_o;
	int		y_o;
	int		iter;
	double	scale;

	scale = SQUARE / ray->distance;
	if (ray->ver)
		x_o = (int)ray->y_wall & (SQUARE - 1);
	else
		x_o = (int)ray->x_wall & (SQUARE - 1);
	y_o = (int)(num_pix * scale);
	iter = 4 * (y_o * SQUARE + x_o);
	color = data_adr[iter];	
	color = color << 8;
	color = color | (data_adr[++iter] & 0xff);
	color = color << 8;
	color = color | (data_adr[++iter] & 0xff);
	color = color << 8;
	return (color);	
}

int		add_shadow(t_wolf *wolf, int color, long double H)
{
	int new_color;
	double dif;

	dif = wolf->view_len;
	if (H > SQUARE * dif && H < SQUARE * (dif + 0.1))
		color = (color >> 1) & 0x7F7F7F;	
	else if (H > SQUARE * (dif + 0.1) && H < SQUARE * (dif + 0.2))
		color = (color >> 1) & 0x6F6F6F;	
	else if (H > SQUARE * (dif + 0.2) && H < SQUARE * (dif + 0.4))
		color = (color >> 1) & 0x5F5F5F;
	else if (H > SQUARE * (dif + 0.4) && H < SQUARE * (dif + 0.6))
		color = (color >> 1) & 0x4F4F4F;
	else if (H > SQUARE * (dif + 0.6) && H < SQUARE * (dif + 0.8))
		color = (color >> 2) & 0x2F2F2F;
	else if (H > SQUARE * (dif + 0.8) && H < SQUARE * (dif + 1.0))
		color = (color >> 3) & 0x1F1F1F;
	else if (H > SQUARE * (dif + 1.0) && H < SQUARE * (dif + 1.2))
		color = (color >> 3) & 0x0F0F0F;
	else if (H > SQUARE * (dif + 1.2))
		color = 0x0;
	return (color);
}

/*
 * height_line высота столба линии вертикальной текстуры
 * distance расстояние до этой текстуры.
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
			break;
		point.color = get_color_point(ray,
				wolf->walls[ray->number_wall].data_adr, ++num, h);
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

t_ray		get_len_ray_gor(t_wolf *wolf, double k, double b, double angle)
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

t_ray		get_len_ray_ver(t_wolf *wolf, double k, double b, double angle)
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

t_ray		get_wall_height(t_wolf *wolf, double angle)
{
	double	k;
	double	b;
	t_ray	r_v;
	t_ray	r_g;

	k = tan(angle*M_PI/180);
	b = wolf->pl.y_pl - k * wolf->pl.x_pl;
	if (k == 0)
		return (get_len_ray_ver(wolf, k, b, angle));
	r_v = get_len_ray_ver(wolf, k, b, angle);
	r_g = get_len_ray_gor(wolf, k, b, angle);
	return ((r_v.distance < r_g.distance) ? r_v : r_g);
}

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
				wolf->img_you_win.img_ptr, 0 ,0);
		return (1);
	}
	if (wolf->bl_game_over)
	{
		mlx_put_image_to_window(wolf->mlx, wolf->window,
				wolf->img_game_over.img_ptr, 0 ,0);
		return (1);
	}
	if (wolf->bl_pause)
	{
		mlx_put_image_to_window(wolf->mlx, wolf->window,
				wolf->img_start.img_ptr, 0 ,0);
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
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->img.img_ptr, 0 ,0);
	clear_image(&wolf->img);
	return (0);
}

void	press_left(t_wolf *wolf)
{
	wolf->pl.pov -= ANGLE;
	wolf->pl.pov = wolf->pl.pov % 360;
	press_enter(wolf); 
}

void	press_right(t_wolf *wolf)
{
	wolf->pl.pov += ANGLE;
	wolf->pl.pov = wolf->pl.pov % 360;
	press_enter(wolf); 
}

void	find_fire(t_wolf *wolf, int x, int y)
{
	wolf->map[y][x] = 0;
	wolf->view_len += 1;
}

void	delete_int_arr(t_wolf *wolf)
{
	int **iter;	
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
	double dx;
	double dy;
	int		x;
	int		y;

	dx = STEP * cos(wolf->pl.pov*M_PI/180);
	dy = STEP * sin(wolf->pl.pov*M_PI/180);
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
	double dx;
	double dy;
	int		x;
	int		y;

	dx = STEP * cos(wolf->pl.pov*M_PI/180);
	dy = STEP * sin(wolf->pl.pov*M_PI/180);
	x = (wolf->pl.x_pl - dx * 10) / SQUARE;
	y = (wolf->pl.y_pl - dy * 10) / SQUARE;
	if (check_number_in_map(wolf, x, y))
	{
		wolf->pl.x_pl -= dx;
		wolf->pl.y_pl -= dy;
	}
	press_enter(wolf); 
}

int		move_mouse(int x, int y, t_wolf *wolf)
{
	static int pre_x = 0;

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
	ft_printf("view_len = [%f]\n", wolf->view_len);
}

void	reduce_scale(t_wolf *wolf)
{
	wolf->view_len -= 0.1;
	ft_printf("view_len = [%f]\n", wolf->view_len);
	if (wolf->view_len < 0)
		wolf->bl_game_over = 1;
}

int		press_key(int key, t_wolf *wolf)
{
	ft_printf("key = {%d}\n", key);
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
//----------------------------------------------------------------------------------------------
void	calculate_tan_cos(t_wolf *wolf)
{
	int		i;
	double	angle;
	double	delta;
	double	da;

	i = -1;	
	delta = (double)wolf->pl.fov / 2;
	wolf->delta_wid = (double)wolf->pl.fov / WIDTH;
	if (!(wolf->cos_arr = (long double*)malloc(sizeof(long double) * WIDTH)))
		sys_err("Error malloc.\n");
	while(++i < WIDTH)
	{
		wolf->cos_arr[i] = cosl(delta * M_PI / 180);
		delta -= wolf->delta_wid;
	}
}

int		check_time(t_wolf *wolf)
{
	static clock_t start_clock = 0;
	clock_t cur_clock;

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
			wolf.img_start.img_ptr, 0 ,0);
	mlx_hook(wolf.window, 2, 0xfffff, move_camera, &wolf);
	mlx_hook(wolf.window, 6, 0xfffff, move_mouse, &wolf);
	mlx_hook(wolf.window, 17, 0xfffff, close_windows, NULL);
	mlx_loop(wolf.mlx);
	return (0);
}
