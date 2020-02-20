/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphyliss <admin@21-school.ru>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 14:03:07 by dphyliss          #+#    #+#             */
/*   Updated: 2020/02/20 12:03:01 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

void		ft_exit(const char *const str)
{
	ft_putstr_fd(str, 2);
	exit(0);
}

void		ft_free_dual_char(char ***str)
{
	char	**buf;

	buf = *str;
	while (*buf)
	{
		ft_strdel(&(*buf));
		++buf;
	}
	free(*str);
	*str = NULL;
}

void		ft_free_dual_int(int **array, const int height)
{
	int		i;

	i = 0;
	while (i < height)
	{
		free(array[i]);
		array[i] = NULL;
		++i;
	}
	free(array);
	array = NULL;
}

int			check_digit(const char *const str)
{
	int		i;

	i = 0;
	((str[i] == '-') || (str[i] == '+')) ? ++i : 0;
	while (str[i])
	{
		if (!((str[i] >= '0') && (str[i] <= '9')))
			return (0);
		++i;
	}
	return (1);
}

void		read_strings(const int height, t_wolf *const wolf, char **buf)
{
	int		i;
	int		k;

	i = 0;
	while (buf[i])
		++i;
	wolf->width = (i > wolf->width) ? i : wolf->width;
	if (!(wolf->map[height] = (int *)ft_memalloc(sizeof(int) * (i + 1))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	k = 0;
	wolf->map[height][k++] = i;
	while (k <= i)
	{
		if (i != 0 && check_digit(buf[k - 1]))
			wolf->map[height][k] = ft_atoi(buf[k - 1]);
		else if (i != 0)
			wolf->map[height][k] = wolf->side;
		++k;
	}
	ft_free_dual_char(&buf);
}

void		read_file(t_wolf *const wolf, const int fd)
{
	char	*line;
	char	**buf;

	wolf->height = 0;
	wolf->width = 0;
	while (get_next_line(fd, &line) == 1)
	{
		if ((buf = ft_strsplit(line, ' ')) == NULL)
			ft_exit("Error: strsplit return NULL");
		ft_strdel(&line);
		read_strings(wolf->height, wolf, buf);
		++wolf->height;
	}
	wolf->map[wolf->height] = NULL;
}

void		rewrite(int *const dest, const int *const source, const int width)
{
	int		i;

	i = 0;
	while (i++ < width)
		dest[i] = source[i];
}

int			**memory_alloc(t_wolf *wolf, const char *const name)
{
	int		**buf;
	int		i;
	int		fd;

	if (((fd = open(name, O_RDONLY)) == -1))
		ft_exit("Error: Map is invalid.\n");
	if (!(wolf->map = (int **)ft_memalloc(sizeof(int *) * 10000)))
		ft_exit("Error: the memory hasn't been allocated.\n");
	read_file(wolf, fd);
	wolf->height = (1 >= wolf->height) ? 2 : wolf->height + 2;
	wolf->width += 2;
	if (!(buf = (int **)ft_memalloc(sizeof(int *) * (wolf->height + 1))))
		ft_exit("Error: the memory hasn't been allocated.\n");
	i = -1;
	while (++i < wolf->height)
		if (!(buf[i] = (int *)ft_memalloc(sizeof(int) * (wolf->width + 1))))
			ft_exit("Error: the memory hasn't been allocated.\n");
	buf[i] = NULL;
	return (buf);
}

void		parser(t_wolf *wolf, char *name)
{
	int		**buf;
	int		i;
	int		j;

	buf = memory_alloc(wolf, name);
	i = -1;
	while (++i < (wolf->height - 2) && wolf->height != 2)
	{
		j = wolf->map[i][0];
		rewrite(buf[i + 1], wolf->map[i], j);
		while (++j < wolf->width)
			buf[i + 1][j] = wolf->side;
		buf[i + 1][0] = wolf->side;
		buf[i + 1][wolf->width] = wolf->side;
	}
	i = -1;
	while (++i < wolf->width)
	{
		buf[0][i] = wolf->side;
		buf[wolf->height - 1][i] = wolf->side;
	}
	ft_free_dual_int(wolf->map, wolf->height - 2);
	wolf->map = buf;
}
