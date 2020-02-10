/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 08:46:55 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/10 12:18:50 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void	init(t_wolf *wolf)
{
	wolf->pl.x_pl = 80;
	wolf->pl.y_pl = 80;
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

void	press_left(t_wolf *wold)
{
		
}

void	drow_vertical_line(t_wolf *wolf, int x, int h)
{
	int		j;
	t_point point;

	j = (HEIGHT / 2) - (h / 2);
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
	ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	if (wolf->map[y][x] == 1 || wolf->map[y][x - 1] == 1)
	{
		ft_printf("wolf->map[x][y] = {%d}\n", wolf->map[y][x]);
		ft_printf("VER Yes!! Wall x = [%d] y = {%d}\n", x, y);
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
	ft_printf("dif squre x = [%d] y = {%d}\n", x, y);
	if (y >= 7)
		return (1);
	if (wolf->map[y][x] == 1 || wolf->map[y - 1][x] == 1)
	{
		ft_printf("wolf->map[x][y] = {%d}\n", wolf->map[y][x]);
		ft_printf("GOR Yes!! Wall x = [%d] y = {%d}\n", x, y);
		return (1);
	}
	return (0);
}

int		min_len_rey(t_wolf *wolf, t_point *ver, t_point *gor)
{
	int len_rey;	
	int len_rey2;	

	len_rey = sqrt(pow(ver->x - wolf->pl.x_pl, 2) - pow(ver->y - wolf->pl.y_pl, 2));
	len_rey2 = sqrt(pow(gor->x - wolf->pl.x_pl, 2) - pow(gor->y - wolf->pl.y_pl, 2));
	ft_printf("len_rey = [%d] len_rey2 = {%d}\n", len_rey, len_rey2);
	return (0);
}

int		check_wall(t_wolf *wolf, double k, double b)
{
	t_point	ver;
	t_point	gor;

	ver.x = wolf->pl.x_pl - wolf->pl.x_pl % SQUARE + SQUARE;
	while (1)
	{
		ver.y = wolf->pl.y_pl + k * (ver.x - wolf->pl.x_pl);
		if (check_wall_vert(wolf, ver.x, ver.y))
			break ;
		ver.x += SQUARE;
	}
	ft_printf("ver.x = [%d] ver.y = {%d}\n", ver.x, ver.y);
	gor.y = wolf->pl.y_pl - wolf->pl.y_pl % SQUARE + SQUARE;
	//x_gl_g = (y_gl_g - wolf->pl.y_pl) / k + wolf->pl.x_pl;
	if (k == 0)
		return (0);
	while (1)
	{
		gor.x = (gor.y - wolf->pl.y_pl) / k + wolf->pl.x_pl;
		if (check_wall_gor(wolf, gor.x, gor.y))
			break ;
		gor.y += SQUARE;
	}
	ft_printf("gor.x = [%d] gor.y = {%d}\n", gor.x, gor.y);
	min_len_rey(wolf, &ver, &gor);
	return (0);
}

int		get_wall_height(t_wolf *wolf, int i)
{
	double	k;	
	double	b;	

	//k = tan(15.9454*M_PI/180);
	k = tan(0*M_PI/180);
	//k = tan(20*M_PI/180);
	b = wolf->pl.y_pl - k * wolf->pl.x_pl;
	check_wall(wolf, k, b);
	//ft_printf("tan(-30) = {%f}\n", tan(330*M_PI/180));
	exit(0);
	return (0);
}

void	press_enter(t_wolf *wolf)
{
	int		i;
	int		h;

	i = -1;
	h = 100;
	while (++i < WIDTH)
	{
		h = get_wall_height(wolf, i);
		drow_vertical_line(wolf, i, h);
		//h--;
	}
	mlx_put_image_to_window(wolf->mlx, wolf->window, wolf->img_ptr,0 ,0);
}

int		press_key(int key, t_wolf *wolf)
{
	ft_printf("key = {%d}\n", key);
	if (key == K_ESC)
		sys_err("Normal exit.\n");
	if (key == K_LEFT)
		press_left(wolf);
	if (key == K_ENTER)
		press_enter(wolf);
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
