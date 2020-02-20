/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_strings.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:04:15 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:35:07 by widraugr         ###   ########.fr       */
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
