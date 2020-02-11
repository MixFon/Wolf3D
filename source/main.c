/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 08:46:55 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/11 15:25:30 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	init(t_wolf *wolf)
{
	wolf->pl.x_pl = 112;
	wolf->pl.y_pl = 112;
	wolf->pl.angle = 0;
	wolf->bits_adr = 4;
	wolf->size_adr = WIDTH;
	wolf->endian = 0;
	wolf->mlx = mlx_init();
	wolf->window = mlx_new_window(wolf->mlx, WIDTH, HEIGHT, "Wolf3D");
	wolf->img_ptr = mlx_new_image(wolf->mlx, WIDTH, HEIGHT);
	wolf->data_adr = mlx_get_data_addr(wolf->img_ptr,
			&wolf->bits_adr, &wolf->size_adr, &wolf->endian);
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
	i = (point.x * wolf->bits_adr / 8) + (point.y * wolf->size_adr);
	wolf->data_adr[i] = point.color;
	wolf->data_adr[++i] = point.color >> 8;
	wolf->data_adr[++i] = point.color >> 16;
	wolf->data_adr[++i] = 0;
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


void	drow_vertical_line(t_wolf *wolf, int x, double h, double angle)
{
	double		j;
	int		i;
	t_point point;

	j = (HEIGHT / 2) - (h / 2);
	if (angle < 0)
		point.color = 0xFF;
	else
		point.color = 0xFFFF;
	point.x = x;
	while (++j < (HEIGHT / 2) + (h / 2))
	{
		point.y = j;
		put_pixel_adr(wolf, point);
	}
}

int		check_wall_vert(t_wolf *wolf, int x_gl, int y_gl)
{
	int x;
	int y;

	x = x_gl / SQUARE;
	y = y_gl / SQUARE;
	if (x < 0 || y < 0 || y > 6 || x > 6)
		return (1);
	//ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	if (wolf->map[y][x] == 1 || wolf->map[y][x - 1] == 1)
	{
	//	ft_printf("wolf->map[x][y] = {%d}\n", wolf->map[y][x]);
	//	ft_printf("VER Yes!! Wall x = [%d] y = {%d}\n", x, y);
		return (1);
	}
	return (0);
}

int		check_wall_gor(t_wolf *wolf, int x_gl, int y_gl)
{
	int x;
	int y;

	x = x_gl / SQUARE;
	y = y_gl / SQUARE;
	//ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	if (x < 0 || y < 0 || y > 6 || x > 6)
		return (1);
	if (wolf->map[y][x] == 1 || wolf->map[y - 1][x] == 1)
	{
	//	ft_printf("wolf->map[x][y] = {%d}\n", wolf->map[y][x]);
	//	ft_printf("GOR Yes!! Wall x = [%d] y = {%d}\n", x, y);
		return (1);
	}
	return (0);
}

double		get_len_ray_gor(t_wolf *wolf, double k, double b, double angle)
{
	t_point	gor;
	int		tmp;

	if ((angle > 0 && angle < 180) || (angle < -180 && angle > -360))
	{
		tmp = SQUARE;
		gor.y = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE + SQUARE;
	}
	else
	{
		tmp = (-1) * SQUARE;
		gor.y = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE;// + SQUARE;
	}
	if (k == 0)
		return (0);
	while (1)
	{
		gor.x = (gor.y - wolf->pl.y_pl) / k + wolf->pl.x_pl;
		//ft_printf("gor.x = [%d] gor.y = {%d}\n", gor.x, gor.y);
		if (check_wall_gor(wolf, gor.x, gor.y))
			break ;
		gor.y += tmp;
	}
	//ft_printf("gor.x = [%d] gor.y = {%d}\n", gor.x, gor.y);
	return (sqrt(pow(gor.x - wolf->pl.x_pl, 2) + pow(gor.y - wolf->pl.y_pl, 2)));
}

double		get_len_ray_ver(t_wolf *wolf, double k, double b, double angle)
{
	t_point	ver;
	int		tmp;

	if ((angle > -90 && angle < 90) || (angle > 270) || (angle < -270))
	{
		tmp = SQUARE;
		ver.x = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE + SQUARE;
	}
	else
	{
		tmp = (-1) * SQUARE;
		ver.x = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE;// + SQUARE;
	}
	while (1)
	{
		ver.y = wolf->pl.y_pl + k * (ver.x - wolf->pl.x_pl);
		if (check_wall_vert(wolf, ver.x, ver.y))
			break ;
		ver.x += tmp;//SQUARE;
	}
	//ft_printf("ver.x = [%d] ver.y = {%d}\n", ver.x, ver.y);
	return (sqrt(pow(ver.x - wolf->pl.x_pl, 2) + pow(ver.y - wolf->pl.y_pl, 2)));
}

double		get_wall_height(t_wolf *wolf, double angle)
{
	double	k;	
	double	b;
	double	h_v;
	double	h_g;
	int		bl;

	//if (angle > 90)
	//	angle = -1 * (180 - angle);
	k = tan(angle*M_PI/180);
	bl = 0;
	//k = tan(angle);
	//if (angle < 0)
	//	k = k * (-1);
	//k = tan(15.9454*M_PI/180);
	//k = tan(0*M_PI/180);
	b = wolf->pl.y_pl - k * wolf->pl.x_pl;
	h_v = get_len_ray_ver(wolf, k, b, angle);
	h_g = get_len_ray_gor(wolf, k, b, angle);
	//ft_printf("h_v = [%d] h_g = {%d}\n", h_v, h_g);
	return ((h_v < h_g) ? h_v : h_g);
	//ft_printf("tan(-30) = {%f}\n", tan(330*M_PI/180));
}

void	clear_image(t_wolf *wolf)
{
	char	*temp;
	int		i;

	temp = wolf->data_adr;
	i = -1;
	while (++i < WIDTH * 4 * HEIGHT)
		temp[i] = 0;
}

void	press_enter(t_wolf *wolf)
{
	int		i;
	double	h;
	int		z;
	double	angle;
	double	da;
	double	delta;
	double	H;
	double	R;

	i = -1;
	delta = 30.0;
	//h = 100;
	angle = wolf->pl.angle - 30;
	//angle = wolf->pl.angle + M_PI / 6;
	//da = (M_PI / 3) / WIDTH;
	da = 60.0 / WIDTH;
	//exit(0);
	while (++i < WIDTH)
	{
		//ft_printf("da {%lf}\n", da);
		//ft_printf("delta {%f}\n", delta);
		//ft_printf("cos(delta) {%f}\n", cos(delta*M_PI/180));
		h = get_wall_height(wolf, angle);
		//ft_printf("h = {%f}\n", h);
		h = h * cos(delta*M_PI/180);
		h = fabs(h);
		//ft_printf("h = {%d}\n", h);
		if (h == 0)
			h = 0.5;
		H = SQUARE * 928 / h;
		//H = SQUARE * 100 / h ;
		//drow_vertical_line(wolf, i, h, angle);
		//H = ceil(H);
		drow_vertical_line(wolf, i, H, angle);
		angle += da;
		delta -= da;
	}
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->img_ptr,0 ,0);
	clear_image(wolf);
}

void	press_left(t_wolf *wolf)
{
	wolf->pl.angle -= 10;
	wolf->pl.angle = wolf->pl.angle % 360;
	press_enter(wolf); 
}

void	press_right(t_wolf *wolf)
{
	wolf->pl.angle += 10;
	wolf->pl.angle = wolf->pl.angle % 360;
	press_enter(wolf); 
}

void	press_up(t_wolf *wolf)
{
	wolf->pl.x_pl += STEP * cos(wolf->pl.angle*M_PI/180);
	wolf->pl.y_pl += STEP * sin(wolf->pl.angle*M_PI/180);
	ft_printf("angle [%f]\n", wolf->pl.angle);
	ft_printf("dx = [%d] dy = [%d]\n", wolf->pl.x_pl, wolf->pl.y_pl);
	press_enter(wolf); 
}

void	press_down(t_wolf *wolf)
{
	wolf->pl.x_pl -= STEP * cos(wolf->pl.angle*M_PI/180);
	wolf->pl.y_pl -= STEP * sin(wolf->pl.angle*M_PI/180);
	ft_printf("angle [%f]\n", wolf->pl.angle);
	ft_printf("dx = [%d] dy = [%d]\n", wolf->pl.x_pl, wolf->pl.y_pl);
	press_enter(wolf); 
}

int		press_key(int key, t_wolf *wolf)
{
	ft_printf("key = {%d}\n", key);
	if (key == K_ESC)
		sys_err("Normal exit.\n");
	if (key == K_LEFT)
		press_left(wolf);
	if (key == K_RIGHT)
		press_right(wolf);
	if (key == K_ENTER)
		press_enter(wolf);
	if (key == K_UP)
		press_up(wolf);
	if (key == K_DOWN)
		press_down(wolf);
	return (0);
}

int		main(int ac, char **av)
{
	t_wolf wolf;

	if (ac != 2)
		sys_err("Two arguments.\n");
	init(&wolf);
	read_map(&wolf, av[1]);
	mlx_key_hook(wolf.window, press_key, &wolf);
	//mlx_loop_hook(wolf.mlx, work_operators, &wolf);
	mlx_loop(wolf.mlx);
	return (0);
}
