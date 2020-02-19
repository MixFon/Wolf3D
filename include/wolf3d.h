#ifndef WOLF3D_H
# define WOLF3D_H

# include "../libft/libft.h"
# include "../minilibx_macos/mlx.h"
# include "math.h"
# include "time.h"

# define WIDTH			1024
# define HEIGHT			720

# define HALFWID		750
# define HALFHEI		500

# define SQUARE			64

# define STEP			4

# define ANGLE			2

# define K_ENTER		36
# define K_ESC			53
# define K_LEFT			123
# define K_UP			126
# define K_RIGHT		124
# define K_DOWN			125
# define K_Z			6
# define K_X			7
# define K_C			8

# define COUNT_WALLS	10

# define WALLS		"./texture/WALL"

# define WALL1		"./texture/WALL1.xpm"
# define WALL2		"./texture/WALL2.xpm"
# define WALL3		"./texture/WALL3.xpm"
# define WALL4		"./texture/WALL4.xpm"
# define WALL5		"./texture/WALL5.xpm"
# define WALL6		"./texture/WALL6.xpm"
# define WALL7		"./texture/WALL7.xpm"
# define WALL8		"./texture/WALL8.xpm"
# define WALL9		"./texture/WALL9.xpm"
# define WALL10		"./texture/WALL10.xpm"

typedef struct		s_ray
{
	long double		x_wall;
	long double		y_wall;		
	long double		distance;	//растояние до пересечения.
	int				x_squre;
	int				y_squre;
	int				ver : 2;
	int				number_wall;
}					t_ray;

typedef struct		s_player
{
	int				x_pl;
	int				y_pl;
	int				pov;		// point of view (угол направления взгляда)
	int				fov;		// field  of view (угол обзора)
}					s_player;

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

typedef struct		s_wolf
{
	void			*mlx;
	void			*window;
	int				**map;
	long double		*cos_arr;
	long double		delta_wid;
	double			view_len;	//Как далеко прорисовывается окружение.
	double			half_hei;	//Половина высоты, для разгрузки вычислений.
	double			for_squre;	//Коэфициент для того чтобы колонки стали квадратными.
	int				width;
	int				height;
	int				side;
	int				bl_pause : 2;
	int				bl_game_over : 2;
	int				bl_you_win : 2;
	int				bl_compass : 2;
	s_player		pl;
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

void		clear_image(t_image *img_ptr);
void		parser(t_wolf *wolf, char *name);
#endif
