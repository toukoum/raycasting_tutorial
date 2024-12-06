

#include <cube.h> 
void init_cub(t_cub *cub) {


	// Initialisation des valeurs par défaut pour les couleurs
	cub->ceilColor = 255;
	cub->floorColor = 16777215;

	// Dimensions par défaut de la map
	cub->width = M_WIDTH;
	cub->height = M_HEIGHT;

	// Initialisation de la map avec des murs (1) et un espace vide (0)
	const char *default_map[M_HEIGHT] = {
			"1111111111",
			"1000000001",
			"1000001101",
			"1000001001",
			"1000000001",
			"1000010011",
			"1000010001",
			"1000010001",
			"1000010001",
			"1111111111"
		};

	//default_map[M_HEIGHT - 1] = NULL;

	// Allocation de la mémoire pour la map
	cub->map = malloc(sizeof(char *) * cub->height + 1);
	cub->map[cub->height] = NULL;

	for (int i = 0; i < cub->height; i++) {
		cub->map[i] = strdup(default_map[i]);
	}

	// Initialisation des coordonnées du joueur et de son angle de départ
	cub->player.x = 1.1;
	cub->player.y = 1.1;
	cub->player.start_angle = 'E';

	// Initialisation de la mlx
	cub->mlx = mlx_init();
	cub->win = mlx_new_window(cub->mlx, WWIN, HWIN, "Tuto Cub3d");
	cub->gameImg.img = mlx_new_image(cub->mlx, WWIN, HWIN);
	cub->gameImg.width = 0;
	cub->gameImg.height = 0;
	cub->gameImg.addr = mlx_get_data_addr(cub->gameImg.img, &cub->gameImg.bits_per_pixel, &cub->gameImg.line_length, &cub->gameImg.endian);

	cub->mmap.img = mlx_new_image(cub->mlx, WMAP, HMAP);
	cub->mmap.width = 0;
	cub->mmap.height = 0;
	cub->mmap.addr = mlx_get_data_addr(cub->mmap.img, &cub->mmap.bits_per_pixel, &cub->mmap.line_length, &cub->mmap.endian);

	cub->dir.y = 0;
	cub->plane.x = 0;
	cub->plane.y = 0;
	cub->dir.x = 0;
	
}