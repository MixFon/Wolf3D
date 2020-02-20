/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:13:59 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:14:08 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

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
	wolf->height = (1 < wolf->height) ? wolf->height + 2 : wolf->height;
	wolf->height = (wolf->height == 1) ? 3 : wolf->height;
	wolf->height = (wolf->height == 0) ? 2 : wolf->height;
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
