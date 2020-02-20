/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widraugr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:20:48 by widraugr          #+#    #+#             */
/*   Updated: 2020/02/20 16:35:00 by widraugr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "../libft/libft.h"
# include "../minilibx_macos/mlx.h"
# include "math.h"
# include "time.h"

# define WIDTH		1024
# define HEIGHT		720

# define HALFWID	750
# define HALFHEI	500

# define SQUARE		64

# define STEP		4

# define ANGLE		5

# define K_ENTER	36
# define K_ESC		53
# define K_LEFT		123
# define K_UP		126
# define K_RIGHT	124
# define K_DOWN		125
# define K_Z		6
# define K_X		7
# define K_C		8

# define COUNT_WALL	11
# define WALLS		"./texture/WALL"

/*
** distance		растояние до пересечения.
** ver			флаг для определения вертикальной стены.
*/
typedef struct		s_ray
{
	long double		x_wall;
	long double		y_wall;
	long double		distance;
	int				ver : 2;
	int				number_wall;
}					t_ray;
/*
** pov			point of view (угол направления взгляда)
** fov			field  of view (угол обзора)
*/
typedef struct		s_player
{
	int				x_pl;
	int				y_pl;
	int				pov;
	int				fov;
}					t_player;

typedef struct		s_image
{
	int				wid;
	int				hei;
	void			*img_ptr;
	int				bits_adr;
	int				size_adr;
	char			*data_adr;
	int				endian;
}					t_image;
/*
** view_len		Как далеко прорисовывается окружение.
** half_hei		Половина высоты, для разгрузки вычислений.
** for_squre	Коэфициент для того чтобы колонки стали квадратными.
*/
typedef struct		s_wolf
{
	void			*mlx;
	void			*window;
	int				**map;
	long double		*cos_arr;
	long double		delta_wid;
	double			view_len;
	double			half_hei;
	double			for_squre;
	int				width;
	int				height;
	int				side;
	int				bl_pause : 2;
	int				bl_game_over : 2;
	int				bl_you_win : 2;
	int				bl_compass : 2;
	t_player		pl;
	t_image			img;
	t_image			img_start;
	t_image			img_game_over;
	t_image			img_you_win;
	t_image			walls[10];
}					t_wolf;

typedef struct		s_point
{
	int				x;
	int				y;
	int				color;
}					t_point;

/*
** File init.c
*/
void				init_image_in_squere_file_xpm(void *mlx, t_image *img,
					int size_squere, char *path);
void				init_image_in_file_xpm(void *mlx, t_image *img, char *path);
void				init_image(void *mlx, t_image *img, int width, int height);
void				init_image_walls_texture(t_wolf *wolf);
void				init(t_wolf *wolf);
/*
** File add_shadow.c
*/
void				put_pixel_adr(t_wolf *wolf, t_point point);
void				infill_int_map(int *iarr, char **carr);
void				print_int_arr(t_wolf *wolf);
int					get_color_point(t_ray *ray, char *data_adr, int num_pix);
int					add_shadow(t_wolf *wolf, int color, long double h);
/*
** File check_wall.c
*/
void				print_floor(t_wolf *wolf, int x,
					long double height_line, double distance);
int					press_key(int key, t_wolf *wolf);
void				drow_vertical_line(t_wolf *wolf, int x,
		t_ray *ray, double h);
int					check_wall_vert(t_wolf *wolf, t_ray *ray);
int					check_wall_gor(t_wolf *wolf, t_ray *ray);
/*
** File get_wall_height.c
*/
void				direction_gor(t_wolf *wolf, t_ray *gor,
		int *tmp, double angle);
t_ray				get_len_ray_gor(t_wolf *wolf, double k, double angle);
void				direction_ver(t_wolf *wolf, t_ray *ver,
		int *tmp, double angle);
t_ray				get_len_ray_ver(t_wolf *wolf, double k, double angle);
t_ray				get_wall_height(t_wolf *wolf, double angle);
/*
** File press_left.c
*/
void				clear_image(t_image *img_ptr);
int					check_pause_or_game_ever(t_wolf *wolf);
int					press_enter(t_wolf *wolf);
void				press_right(t_wolf *wolf);
void				press_left(t_wolf *wolf);
/*
** File press_down.c
*/
void				find_fire(t_wolf *wolf, int x, int y);
void				delete_int_arr(t_wolf *wolf);
int					check_number_in_map(t_wolf *wolf, int x, int y);
void				press_up(t_wolf *wolf);
void				press_down(t_wolf *wolf);
/*
** File reduce_scale.c
*/
int					move_mouse(int x, int y, t_wolf *wolf);
int					move_camera(int key, t_wolf *wolf);
int					close_windows(void);
void				increase_scale(t_wolf *wolf);
void				reduce_scale(t_wolf *wolf);
/*
** File main.c
*/
void				calculate_tan_cos(t_wolf *wolf);
int					check_time(t_wolf *wolf);
void				infill_position(t_wolf *wolf, int i, int j);
void				start_position(t_wolf *wolf);
int					main(int ac, char **av);
/*
** ------ Parser ------
*/
/*
** File read_strings.c.
*/
void				ft_exit(const char *const str);
void				ft_free_dual_char(char ***str);
void				ft_free_dual_int(int **array, const int height);
int					check_digit(const char *const str);
void				read_strings(const int height, t_wolf *const wolf,
		char **buf);
/*
** File parser.c.
*/
void				read_file(t_wolf *const wolf, const int fd);
void				rewrite(int *const dest, const int *const source,
		const int width);
int					**memory_alloc(t_wolf *wolf, const char *const name);
void				parser(t_wolf *wolf, char *name);
#endif
