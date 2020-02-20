/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_shadow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:44:08 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:06:22 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/wolf3d.h"

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
	ft_printf("Map:\n");
	while (++i < wolf->height)
	{
		j = -1;
		while (++j < wolf->width)
			ft_printf("%d ", wolf->map[i][j]);
		ft_putchar('\n');
	}
}

int		get_color_point(t_ray *ray, char *data_adr, int num_pix)
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

int		add_shadow(t_wolf *wolf, int color, long double h)
{
	double	dif;

	dif = wolf->view_len;
	if (h > SQUARE * dif && h < SQUARE * (dif + 0.1))
		color = (color >> 1) & 0x7F7F7F;
	else if (h > SQUARE * (dif + 0.1) && h < SQUARE * (dif + 0.2))
		color = (color >> 1) & 0x6F6F6F;
	else if (h > SQUARE * (dif + 0.2) && h < SQUARE * (dif + 0.4))
		color = (color >> 1) & 0x5F5F5F;
	else if (h > SQUARE * (dif + 0.4) && h < SQUARE * (dif + 0.6))
		color = (color >> 1) & 0x4F4F4F;
	else if (h > SQUARE * (dif + 0.6) && h < SQUARE * (dif + 0.8))
		color = (color >> 2) & 0x2F2F2F;
	else if (h > SQUARE * (dif + 0.8) && h < SQUARE * (dif + 1.0))
		color = (color >> 3) & 0x1F1F1F;
	else if (h > SQUARE * (dif + 1.0) && h < SQUARE * (dif + 1.2))
		color = (color >> 3) & 0x0F0F0F;
	else if (h > SQUARE * (dif + 1.2))
		color = 0x0;
	return (color);
}
