
#ifndef __CUBE_H__
# define __CUBE_H__

# ifdef __APPLE__
#  define LINUX 0
#  define MY_DIR O_DIRECTORY
# elif __linux__
#  include <X11/keysym.h>
#  define LINUX 1
#  define MY_DIR __O_DIRECTORY
# endif

# include <X11/X.h>
# include <fcntl.h> // open
# include <math.h>
# include <mlx.h>
# include <stdbool.h> // boolean
# include <stdlib.h>  // open, write
# include <unistd.h>  // open, write
# include <string.h>
# include <stdio.h>

// =========================== EXIT CODE ===========================

# define WWIN 1280            // width of window
# define HWIN 720             // height of window
# define WMAP 20 * WWIN / 100 // width of minimap
# define HMAP 20 * HWIN / 100 // height of minimap

# define TSIZE 15 // taille d'un carreau de la minimap

# define M_HEIGHT 10
# define M_WIDTH 10

# define ALL_GOOD 0
# define ALL_PAS_GOOD 1
# define MALLOC_ERROR 2

// color for mlx
# define CWHITE 0xFFFFFF
# define CGREEN 0x28CD41
# define CRED 0xFF3B30
# define CBLACK 0x000000
# define CCYAN 0x5AC8FA
# define CMAGENTA 0xFF2D55
# define CYELLOW 0xFFCC00
# define CORANGE 0xFF9500
# define CPINK 0xFF2D55
# define CLIME 0x34C759

# define CWALL 0x1C1C1E
# define CGROUND 0xF2F2F7
# define CUNDEFINED 0x8E8E93
# define CDOOR 0x007AFF
# define CPLAYER 0xFF375F

# define CRAY 0xD1D1D6

# define MOVESPEED 0.06
# define ROTSPEED 0.1
# define CHANGEFOV 2
# define SENSITIVITY 0.0000001

# define HITBOX_RADIUS 0.6

# define CENTER_MMAP_X WMAP / 2 - TSIZE / 2
# define CENTER_MMAP_Y HMAP / 2 - TSIZE / 2

// =========================== STRUCT ===========================

typedef struct s_coord
{
	double		x;
	double		y;
}				t_coord;

typedef struct s_int_coord
{
	int			x;
	int			y;
}				t_int_coord;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_img;

typedef struct s_ray
{
	int			line_height;
	t_coord		rayDir;
	t_coord		d;
	t_int_coord	map;
	t_coord		sideDist;
	double		perpWallDist;
	t_int_coord	step;
	int			side_hit;
	int			draw_start;
	int			draw_end;
	int 		draw_x;
	int			color;
}				t_ray;

typedef struct s_player
{
	double		x;
	double		y;
	char start_angle; // 'N', 'S', 'E', 'W'
}				t_player;

typedef struct s_cub
{
	void *mlx; // MinilibX
	void		*win;

	char **map;
	int floorColor;  // couleur sol et plafond
	int	ceilColor;
	int width;  // largeur de la map
	int height; // hauteur de la map

	t_player player; // pos x, y

	t_img gameImg; // main img of the whole window
	t_img mmap;    // img of the minimap

	t_coord		dir;
	t_coord		plane;
}				t_cub;

// =========================== EVENT MLX ===========================

enum
{
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

// =========================== Functions ===========================

int				handle_key(int keycode, t_cub *cub);
int				handle_close_win(t_cub *cub);
void			free_cub(t_cub *cub);
void			quit_cub(int exit_code);
void			init_cub(t_cub *cub);
void			event(t_cub *cub);
void			render(t_cub *cub);
void			rotate_player(int keycode, t_cub *cub);
void			move_player(int keycode, t_cub *cub);

// =========================== DEFINE ===========================

# ifdef __APPLE__
#  define XK_Escape 53
#  define XK_q 12
#  define XK_w 13
#  define XK_e 14
#  define XK_a 0
#  define XK_s 1
#  define XK_d 2
#  define XK_h 4
#  define XK_Up 126
#  define XK_Down 125
#  define XK_Left 123
#  define XK_Right 124
#  define XK_plus 24
#  define XK_minus 27
#  define XK_space 49

# endif

#endif
