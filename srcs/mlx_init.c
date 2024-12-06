#include <cube.h>

void	quit_cub(int exit_code)
{
	if (exit_code == MALLOC_ERROR)
	{
		perror("Malloc Error");
		exit(EXIT_FAILURE);
	}
	else if (exit_code == EXIT_SUCCESS)
	{
		printf("Program quit successfully !");
		exit(EXIT_SUCCESS);
	}
}

void	free_cub(t_cub *cub)
{
	mlx_destroy_window(cub->mlx, cub->win);
	// mlx_destroy_display(cub->mlx);
	free(cub->mlx);
}

int	handle_close_win(t_cub *cub)
{
	free_cub(cub);
	quit_cub(EXIT_SUCCESS);
	return (ALL_GOOD);
}

int	handle_key(int keycode, t_cub *cub)
{
	if (keycode == XK_Escape)
		return (free_cub(cub), quit_cub(EXIT_SUCCESS), ALL_GOOD);
	if (XK_w == keycode)
		move_player(XK_w, cub);
	if (XK_s == keycode)
		move_player(XK_s, cub);
	if (XK_a == keycode)
		move_player(XK_a, cub);
	if (XK_d == keycode)
		move_player(XK_d, cub);
	if (XK_Left == keycode)
		rotate_player(XK_Left, cub);
	if (XK_Right == keycode)
		rotate_player(XK_Right, cub);
	render(cub);
	return (ALL_GOOD);
}

void	event(t_cub *cub)
{
	mlx_hook(cub->win, ON_DESTROY, StructureNotifyMask, handle_close_win, cub);
	mlx_hook(cub->win, ON_KEYDOWN, KeyPressMask, handle_key, cub);
}