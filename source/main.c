
#include "../include/wolf3d.h"

void	init(t_wolf *wolf)
{
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

int		press_key(int key, t_wolf *wolf)
{
	ft_printf("key = {%d}\n", key);
	if (key == K_ESC)
		sys_err("Normal exit.\n");
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
