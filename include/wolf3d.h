#ifndef WOLF3D_H
# define WOLF3D_H

# include "../libft/libft.h"
# include "../minilibx_macos/mlx.h"
# include "math.h"

# define WIDTH		1024
# define HEIGHT		720

# define HALFWID	750
# define HALFHEI	500

# define SQUARE		64

# define STEP		4

# define K_ENTER	36
# define K_ESC		53
# define K_LEFT		123
# define K_UP		126
# define K_RIGHT	124
# define K_DOWN		125

typedef struct		s_node
{
	int				data;
	int				number;
	struct s_node	*next;
}					t_node;

typedef struct		s_player
{
	int				x_pl;
	int				y_pl;
	int				pov;		// point of view (угол направления взгляда)
	int				fov;		// field  of view (угол обзора)
}					s_player;

typedef struct		s_wolf
{
	void			*mlx;
	void			*window;
	void			*img_ptr;
	int				bits_adr;
	int				size_adr;
	int				endian;
	char			*data_adr;
	int				**map;
	long double		*cos_arr;
	//long double		delta_fov;
	long double		delta_wid;
	s_player		pl;
}					t_wolf;

typedef struct		s_point
{
	int				x;
	int				y;
	int				color;
}					t_point;

#endif
