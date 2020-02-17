/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 08:46:55 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/17 16:59:15 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	init_image_in_file_xpm(void *mlx, t_image *img, int size_squere , char *path)
{
	img->wid = size_squere;
	img->hei = size_squere;
	img->bits_adr = 4;
	img->size_adr = size_squere;
	img->endian = 0;
	if (!(img->img_ptr = mlx_xpm_file_to_image(mlx, path, &img->wid, &img->hei)))
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
		init_image_in_file_xpm(wolf->mlx, &wolf->walls[i], SQUARE, name_wall);
		ft_printf("number = {%s}, name_wall = [%s]\n", number, name_wall);
		ft_strdel(&number);
		ft_strdel(&name_wall);
	}
}

void	init(t_wolf *wolf)
{
	wolf->pl.x_pl = 224;
	wolf->pl.y_pl = 224;
	wolf->pl.pov = 0;
	wolf->pl.fov = 60.0;
	wolf->cos_arr = NULL;
	wolf->delta_wid = 0.0;
	wolf->view_len = 2.0 * SQUARE;
	wolf->half_hei = HEIGHT / 2;
	wolf->for_squre = SQUARE * 928;
	//	wolf->delta_fov = 0.0;
	wolf->mlx = mlx_init();
	wolf->window = mlx_new_window(wolf->mlx, WIDTH, HEIGHT, "Wolf3D");
	init_image(wolf->mlx, &wolf->img, WIDTH, HEIGHT);
	init_image_walls_texture(wolf);
}

t_point	ft_draw_line_source(t_point *delta,
		t_point *sign, t_point point1, t_point point2)
{
	(*delta).x = abs((point2.x - point1.x));
	(*delta).y = abs((point2.y - point1.y));
	(*sign).x = (point1.x < point2.x) ? 1 : -1;
	(*sign).y = (point1.y < point2.y) ? 1 : -1;
	return (point1);
}

void	put_pixel_adr(t_wolf *wolf, t_point point)
{
	int i;

	if (point.x >= WIDTH || point.y >= HEIGHT || point.x <= 0 || point.y <= 0)
		return ;
	i = (point.x * wolf->img.bits_adr / 8) + (point.y * wolf->img.size_adr);
	//wolf->img.data_adr[i] = point.color;
	//ft_printf("color [%#x]\n", point.color);
	//strncpy(&wolf->img.data_adr[i], (const char*)&point.color, 4);
	wolf->img.data_adr[i] = point.color >> 24;
	//ft_printf("c [%#x]\n", wolf->img.data_adr[i]);
	wolf->img.data_adr[++i] = point.color >> 16;
	//ft_printf("c [%#x]\n", wolf->img.data_adr[i]);
	wolf->img.data_adr[++i] = point.color >> 8;
	//ft_printf("c [%#x]\n", wolf->img.data_adr[i]);
	wolf->img.data_adr[++i] = 0;
	//ft_printf("c [%#x]\n", wolf->img.data_adr[i]);
	//exit(0);
}

void	ft_draw_line(t_wolf *wolf, t_point point1, t_point point2, int color)
{
	t_point	delta;
	t_point	sign;
	t_point	point;
	int		error;
	int		error2;

	point = ft_draw_line_source(&delta, &sign, point1, point2);
	error = delta.x - delta.y;
	put_pixel_adr(wolf, point2);
	while (point.x != point2.x || point.y != point2.y)
	{
		put_pixel_adr(wolf, point);
		error2 = error * 2;
		if (error2 > -delta.y)
		{
			error -= delta.y;
			point.x += sign.x;
		}
		if (error2 < delta.x)
		{
			error += delta.x;
			point.y += sign.y;
		}
		point.color = color;
	}
}

void	infill_int_map(int *iarr, char **carr)
{
	int i;
	char **tmp;

	i = -1;
	tmp = carr;
	while (*carr != NULL)
	{
		iarr[++i] = ft_atoi(*carr);
		ft_strdel(carr);
		carr++;
		//ft_printf("%d ", iarr[i]);
	}
	free(tmp);
	//ft_putchar('\n');
}

void	print_int_arr(int **map)
{
	int i;
	int j;

	i = -1;
	while (++i < 7)
	{
		j = -1;
		while (++j < 7)
			ft_printf("%d ", map[i][j]);
		ft_putchar('\n');
	}
}

void	read_map(t_wolf *wolf, char *name)
{
	int		fd;
	char	*line;
	int		count;
	char	**arr;
	int		i;

	line = NULL;
	i = 0;
	if ((fd = open(name, O_RDONLY)) == -1)
		sys_err("Error open file.\n");
	get_next_line(fd, &line);
	count = ft_strlen(line);
	if (!(wolf->map = (int **)malloc(sizeof(int*) * count)))
		sys_err("Error malloc\n");
	if (!(wolf->map[0] = (int *)malloc(sizeof(int) * count)))
		sys_err("Error malloc\n");
	arr = ft_strsplit(line, ' ');
	infill_int_map(wolf->map[0], arr);
	//delete_arr(&arr);
	ft_strdel(&line);
	while(get_next_line(fd, &line))
	{
		if (!(wolf->map[++i] = (int *)malloc(sizeof(int) * count)))
			sys_err("Error malloc\n");
		arr = ft_strsplit(line, ' ');
		infill_int_map(wolf->map[i], arr);
		ft_strdel(&line);
	}
	print_int_arr(wolf->map);
	//exit(0);
}

void	print_data_adr(char *data_adr)
{
	int i;

	i = -1;
	while(++i < 64 * 10)
		ft_printf("c = [%#x] i = [%d]\n",data_adr[i], i);
}
int		get_color_point(t_ray *ray, char *data_adr, int num_pix, double H)
{
	int		color;
	int		x_o;
	int		y_o;
	int		iter;
	double	scale;

	scale = SQUARE / ray->distance;
	//scale = ray->distance / (SQUARE);
	//ft_printf("scale [%f]\n", scale);
	//exit(0);
	//scale = 0.3;
	if (ray->ver)
		x_o = (int)ray->y_wall & (SQUARE - 1);
	else
		x_o = (int)ray->x_wall & (SQUARE - 1);
	//x_o = ray->y_squre;
	y_o = (int)(num_pix * scale);
	iter = 4 * (y_o * SQUARE + x_o);
	//ft_printf("num_pix {%d} x_o = [%d] y_o = [%d]\n", num_pix,x_o, y_o);
	//ft_printf("x_o = [%d] y = [%d] iter = {%d}\n", x_o, y_o, iter);
	//exit(0);
	//ft_printf("color [%#x]\n", color);
	//iter--;
	//iter = 0;
	color = data_adr[iter];	
	//ft_printf("color [%#x]\n", color);
	color = color << 8;
	//ft_printf("color [%#x]\n", color);
	color = color | (data_adr[++iter] & 0xff);
	//ft_printf("color [%#x]\n", color);
	color = color << 8;
	//ft_printf("color [%#x]\n", color);
	color = color | (data_adr[++iter] & 0xff);
	//ft_printf("color [%#x]\n", color);
	color = color << 8;
	//else if (H > SQUARE * (dif + 1.5))
	//	color = (color >> 1) & 0x0F0F0F;
	//ft_printf("color [%#x]\n", color);
	//print_data_adr(data_adr);
	//exit(0);
	return (color);	
}

int		add_shadow(t_wolf *wolf, int color, double H)
{
	int new_color;
	double dif;

	dif = 3.5;
	if (H > SQUARE * dif && H < SQUARE * (dif + 0.1))
		color = (color >> 1) & 0x7F7F7F;	
	else if (H > SQUARE * (dif + 0.1) && H < SQUARE * (dif + 0.2))
		color = (color >> 1) & 0x6F6F6F;	
	else if (H > SQUARE * (dif + 0.2) && H < SQUARE * (dif + 0.4))
		color = (color >> 1) & 0x5F5F5F;
	else if (H > SQUARE * (dif + 0.4) && H < SQUARE * (dif + 0.6))
		color = (color >> 1) & 0x4F4F4F;
	else if (H > SQUARE * (dif + 0.6) && H < SQUARE * (dif + 0.8))
		color = (color >> 1) & 0x3F3F3F;
	else if (H > SQUARE * (dif + 0.8) && H < SQUARE * (dif + 1.0))
		color = (color >> 1) & 0x2F2F2F;
	else if (H > SQUARE * (dif + 1.1) && H < SQUARE * (dif + 1.2))
		color = (color >> 1) & 0x1F1F1F;
	else if (H > SQUARE * (dif + 1.2) && H < SQUARE * (dif + 1.3))
		color = (color >> 1) & 0x0F0F0F;
	return (color);
}
void	drow_vertical_line(t_wolf *wolf, int x, t_ray *ray, double H)
{
	double	j;
	int		num_pix;
	int		len;
	t_point point;
	
	num_pix = -1;
	j = wolf->half_hei - (ray->distance / 2.0);
	if (j < 0)
	{
		num_pix = (ray->distance - HEIGHT) / 2;
		j = 0;
	}
	//point.color = 0xFFFF;
	point.x = x;
	len = wolf->half_hei + (ray->distance / 2.0);
	while (++j < len)
	{
		if (j > HEIGHT)
			break;
		point.color = get_color_point(ray, wolf->walls[ray->number_wall].data_adr, ++num_pix, H);
		point.color = add_shadow(wolf, point.color, H);
		point.y = j;
		put_pixel_adr(wolf, point);
	}
}

int		check_wall_vert(t_wolf *wolf, t_ray *ray) //int x_gl, int y_gl)
{
	int x;
	int y;

	x = ray->x_wall / SQUARE;
	y = ray->y_wall / SQUARE;
	//ray->number_wall = 0;
	if (x < 0 || y < 0 || y > 6 || x > 6)
		return (1);
	//ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	if (wolf->map[y][x] != 0)
	{
		//ray->number_wall = wolf->map[y][x];
		return (1);
	}
	if ( wolf->map[y][x - 1] != 0)
	{
		//ray->number_wall = wolf->map[y][x - 1];
		return (1);
	}
	return (0);
}

int		check_wall_gor(t_wolf *wolf, t_ray *ray)//int x_gl, int y_gl)
{
	int x;
	int y;

	x = ray->x_wall / SQUARE;
	y = ray->y_wall / SQUARE;
	//ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	//ray->number_wall = 0;
	if (x < 0 || y < 0 || y > 6 || x > 6)
		return (1);
	if (wolf->map[y][x] != 0)
	{
		//ray->number_wall = wolf->map[y][x];
		return (1);
	}
	if (wolf->map[y - 1][x] != 0)
	{
		//ray->number_wall = wolf->map[y - 1][x];
		return (1);
	}
	return (0);
}

t_ray		get_len_ray_gor(t_wolf *wolf, double k, double b, double angle)
{
	t_ray	gor;
	int		tmp;

	if ((angle > 0 && angle < 180) || (angle < -180 && angle > -360) || angle > 360)
	{
		tmp = SQUARE;
		gor.y_wall = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE + SQUARE;
		gor.number_wall = 4;
	}
	else
	{
		tmp = (-1) * SQUARE;
		gor.y_wall = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE;// + SQUARE;
		gor.number_wall = 3;
	}
	while (1)
	{
		gor.x_wall = (gor.y_wall - wolf->pl.y_pl) / k + wolf->pl.x_pl;
		//ft_printf("gor.x = [%d] gor.y = {%d}\n", gor.x_wall, gor.y_wall);
		if (check_wall_gor(wolf, &gor))// gor.x_wall, gor.y_wall))
			break ;
		gor.y_wall += tmp;
	}
	//ft_printf("gor.x = [%d] gor.y = {%d}\n", gor.x, gor.y);
	gor.ver = 0;
	gor.x_squre = (int)gor.x_wall & (SQUARE - 1);
	gor.distance = sqrt(pow(gor.x_wall - wolf->pl.x_pl, 2) + pow(gor.y_wall - wolf->pl.y_pl, 2));
	return (gor);
}

t_ray		get_len_ray_ver(t_wolf *wolf, double k, double b, double angle)
{
	t_ray	ver;
	int		tmp;

	if ((angle > -90 && angle < 90) || (angle > 270) || (angle < -270))
	{
		tmp = SQUARE;
		ver.x_wall = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE + SQUARE;
		ver.number_wall = 1;
	}
	else
	{
		tmp = (-1) * SQUARE;
		ver.x_wall = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE;// + SQUARE;
		ver.number_wall = 2;
	}
	while (1)
	{
		ver.y_wall = wolf->pl.y_pl + k * (ver.x_wall - wolf->pl.x_pl);
		if (check_wall_vert(wolf, &ver))// ver.x_wall, ver.y_wall))
			break ;
		ver.x_wall += tmp;//SQUARE;
	}
	//ft_printf("ver.x = [%d] ver.y = {%d}\n", ver.x, ver.y);
	//ft_printf("ver.y = [%d] \n",ver.y & 0x1f);
	ver.ver = 1;
	ver.distance = sqrt(pow(ver.x_wall - wolf->pl.x_pl, 2) + pow(ver.y_wall - wolf->pl.y_pl, 2));
	return (ver);
}

t_ray		get_wall_height(t_wolf *wolf, double angle)
{
	double	k;
	double	b;
	t_ray	r_v;
	t_ray	r_g;

	//if (angle > 90)
	//	angle = -1 * (180 - angle);
	k = tan(angle*M_PI/180);
	b = wolf->pl.y_pl - k * wolf->pl.x_pl;
	if (k == 0)
		return (get_len_ray_ver(wolf, k, b, angle));
	//k = tan(angle);
	//if (angle < 0)
	//	k = k * (-1);
	//k = tan(15.9454*M_PI/180);
	//k = tan(0*M_PI/180);
	r_v = get_len_ray_ver(wolf, k, b, angle);
	r_g = get_len_ray_gor(wolf, k, b, angle);
	//ft_printf("h_v = [%d] h_g = {%d}\n", h_v, h_g);
	return ((r_v.distance < r_g.distance) ? r_v : r_g);
	//ft_printf("tan(-30) = {%f}\n", tan(330*M_PI/180));
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

int		press_enter(t_wolf *wolf)
{
	int		i;
	t_ray	ray;
	double	angle;
	double	H;
	double	R;

	i = -1;
	angle = wolf->pl.pov - wolf->pl.fov / 2; //Определяем начальный угол отрисовки.
	//angle = wolf->pl.angle + M_PI / 6;
	while (++i < WIDTH)
	{
		//ft_printf("cos(delta) {%f}\n", cos(delta*M_PI/180));
		ray = get_wall_height(wolf, angle);
		//ft_printf("h = {%f}\n", h);
		//h = h * cos(delta*M_PI/180);
		H = ray.distance;
		ray.distance = ray.distance * wolf->cos_arr[i];
		if (H > wolf->view_len * 2)
		{
			angle += wolf->delta_wid;
			continue;
		}
		//h = fabs(h);
		//ft_printf("h = {%d}\n", h);
		//if (h == 0)
		//	h = 1;
		//ray.distance = SQUARE * 928.0 / ray.distance;
		ray.distance = wolf->for_squre / ray.distance;
		//ft_printf("ray.distance = {%Lf}\n", ray.distance);
		//ft_printf("distance = {%Lf}\n", ray.distance);
		drow_vertical_line(wolf, i, &ray, H);
		angle += wolf->delta_wid;
	}
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->img.img_ptr,0 ,0);
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->walls[0].img_ptr, 0 ,0);
//	clear_image(&wolf->wall);
	clear_image(&wolf->img);
	return (0);
}

void	press_left(t_wolf *wolf)
{
	wolf->pl.pov -= 4;
	wolf->pl.pov = wolf->pl.pov % 360;
	//press_enter(wolf); 
}

void	press_right(t_wolf *wolf)
{
	wolf->pl.pov += 4;
	wolf->pl.pov = wolf->pl.pov % 360;
	//press_enter(wolf); 
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
	//ft_printf("x = [%d] y = [%d] map = [%d]\n", y, x, wolf->map[y][x]);
	if (wolf->map[y][x] == 0)
	{
		wolf->pl.x_pl += dx;
		wolf->pl.y_pl += dy;
	}
	//ft_printf("angle [%f]\n", wolf->pl.pov);
	//ft_printf("dx = [%d] dy = [%d]\n", wolf->pl.x_pl, wolf->pl.y_pl);
	//press_enter(wolf); 
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
	//ft_printf("x = [%d] y = [%d] map = [%d]\n", y, x, wolf->map[y][x]);
	if (wolf->map[y][x] == 0)
	{
		wolf->pl.x_pl -= dx;
		wolf->pl.y_pl -= dy;
	}
	//ft_printf("dx = [%d] dy = [%d]\n", wolf->pl.x_pl, wolf->pl.y_pl);
	//press_enter(wolf); 
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
	if (key == K_UP)
		press_up(wolf);
	if (key == K_DOWN)
		press_down(wolf);
	if (key == K_LEFT)
		press_left(wolf);
	if (key == K_RIGHT)
		press_right(wolf);
	return (0);
}

int		press_mouse(int button, int x, int y, t_wolf *wolf)
{
	ft_printf("button = {%d} x = [%d] y = {%d}\n", button, x, y);
	if (x > 1 && x < 21 && y > -2 && y < -21)
		sys_err("Normal exit.\n");
	return (0);
}

int		close_windows(void)
{
	sys_err("Normal exit.\n");
	return (0);
}

int		press_key(int key, t_wolf *wolf)
{
	ft_printf("key = {%d}\n", key);
	if (key == K_ESC)
		sys_err("Normal exit.\n");
	return (0);
}

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

int		main(int ac, char **av)
{
	t_wolf wolf;

	if (ac != 2)
		sys_err("Two arguments.\n");
	init(&wolf);
	read_map(&wolf, av[1]);
	calculate_tan_cos(&wolf);
	mlx_key_hook(wolf.window, press_key, &wolf);
	mlx_loop_hook(wolf.mlx, press_enter, &wolf);
	mlx_mouse_hook(wolf.window, press_mouse, &wolf);
	//mlx_hook(wolf.window, 3, 0xfffff, move_camera, &wolf);
	mlx_hook(wolf.window, 2, 0xfffff, move_camera, &wolf);
	mlx_hook(wolf.window, 6, 0xfffff, move_mouse, &wolf);
	mlx_hook(wolf.window, 17, 0xfffff, close_windows, NULL);
	mlx_loop(wolf.mlx);
	return (0);
}
