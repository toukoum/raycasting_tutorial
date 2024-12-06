#include <cube.h>

// =========================== Utils functions ===========================

void	draw_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

/**
 * @brief trace une ligne entre deux points
 * Calcul de la pente de la droite entre les deux points
 * puis on trace la ligne en incrémentant x et y
 * 
 * @param x1 point de départ x et y
 * @param x2 point d'arrivée x et y
 */
void draw_line(t_img *img, t_coord x1, t_coord x2, int color) {
    t_coord d;
    t_coord increment;
    double dmax;
    int i;

    d.x = x2.x - x1.x;
    d.y = x2.y - x1.y;
    dmax = fmax(fabs(d.x), fabs(d.y));
    increment.x = d.x / dmax;
    increment.y = d.y / dmax;

    for (i = 0; i <= dmax; i++) {
        if (x1.x > 0 && x1.y > 0)
            draw_pixel(img, (int)x1.x, (int)x1.y, color);
        x1.x += increment.x;
        x1.y += increment.y;
    }
}

// Function to check if map index is out of bounds
bool	is_out_of_bounds(t_int_coord *map_idx, t_cub *cub)
{
	return (map_idx->x < 0 || map_idx->x >= cub->width || map_idx->y < 0
		|| map_idx->y >= cub->height);
}

void	print_log_debug(t_cub *cub)
{
	int	i;
	int	j;

	i = -1;
	printf("\n===== DEBUG MODE ====\n\n");
	printf("> height/width of map: %d, %d\n", cub->height, cub->width);
	printf("> pos of player (x, y): %f, %f\n", cub->player.x, cub->player.y);
	printf("> map: \n\n");
	while (++i < cub->height)
	{
		j = -1;
		while (cub->map[i][++j]){
			printf("%c", cub->map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// =========================== PLAYER ===========================

void init_start_angle(t_cub *cub) {
    switch (cub->player.start_angle) {
        case 'N':
            cub->dir.y = -1;
            cub->plane.x = 0.66;
            break;
        case 'S':
            cub->dir.y = 1;
            cub->plane.x = -0.66;
            break;
        case 'E':
            cub->dir.x = 1;
            cub->plane.y = 0.66;
            break;
        case 'W':
            cub->dir.x = -1;
            cub->plane.y = -0.66;
            break;
    }
}

/**
 * @brief Utilise la matrice de rotation pour tourner le joueur
 * fait tourner le plan et la direction du joueur
 */
static void rotate(t_cub *cub, double angle) 
{
    double old_dir_x = cub->dir.x;
    double old_plane_x = cub->plane.x;
    double cos_r = cos(angle);
    double sin_r = sin(angle);

    cub->dir.x = cub->dir.x * cos_r - cub->dir.y * sin_r;
    cub->dir.y = old_dir_x * sin_r + cub->dir.y * cos_r;
    cub->plane.x = cub->plane.x * cos_r - cub->plane.y * sin_r;
    cub->plane.y = old_plane_x * sin_r + cub->plane.y * cos_r;
}

void rotate_player(int keycode, t_cub *cub) {
    if (keycode == XK_Left)
        rotate(cub, -ROTSPEED);
    else if (keycode == XK_Right)
        rotate(cub, ROTSPEED);
}


void	add_move(t_cub *cub, int keycode, t_coord *next)
{
	if (keycode == XK_w){
		next->x += MOVESPEED * cub->dir.x;
		next->y += MOVESPEED * cub->dir.y;
	}
	else if (keycode == XK_s){
		next->x -= MOVESPEED * cub->dir.x;
		next->y -= MOVESPEED * cub->dir.y;
	}
	else if (keycode == XK_d){
		next->x -= MOVESPEED * cub->dir.y;
		next->y += MOVESPEED * cub->dir.x;
	}
	else if (keycode == XK_a){
		next->x += MOVESPEED * cub->dir.y;
		next->y -= MOVESPEED * cub->dir.x;
	}
}

/**
 * @brief Calcule la prochaine position du joueur
 * et vérifie si le joueur peut se déplacer à cette position
 * Si oui, le joueur se déplace à cette position
 */
void	move_player(int keycode, t_cub *cub)
{
	t_coord		next;
	t_int_coord	map_index;

	next.x = cub->player.x;
	next.y = cub->player.y;
	add_move(cub, keycode, &next);
	map_index.x = (int)(next.x);
	map_index.y = (int)(next.y);
	if (cub->map[map_index.y][map_index.x] == '0'
		|| cub->map[map_index.y][map_index.x] == cub->player.start_angle)
	{
		cub->player.x = next.x;
		cub->player.y = next.y;
		printf("player x: %f, y: %f\n", cub->player.x, cub->player.y);
	}
}



// =========================== Minimap ===========================

// Function to get the color of a pixel on the minimap
int get_pixel_color(char **map, t_int_coord *mapIndex) {
    char tile;

    tile = map[mapIndex->y][mapIndex->x];
    switch (tile) {
        case '1': return CWALL;
        case '0': return CGROUND;
        case '2': return CWHITE;
        default:
            return (is_space(tile)) ? CUNDEFINED : CGREEN;
    }
}

void draw_minimap_pixel(t_cub *cub, t_coord player, int x, int y)
{
    double adj_x = player.x - (WMAP / 2) + x;
    double adj_y = player.y - (HMAP / 2) + y;

    t_int_coord map_idx = {
        .x = (int)round(adj_x / TSIZE),
        .y = (int)round(adj_y / TSIZE)
    };

    int color = is_out_of_bounds(&map_idx, cub) ? CUNDEFINED : get_pixel_color(cub->map, &map_idx);
    draw_pixel(&cub->mmap, x, y, color);
}

void draw_minimap(t_cub *cub, t_coord player)
{
    for (int y = 0; y < HMAP; y++)
        for (int x = 0; x < WMAP; x++)
            draw_minimap_pixel(cub, player, x, y);
		
		for (int i = 0; i < WMAP; i++) {
			draw_pixel(&cub->mmap, i, 0, CWHITE);
			draw_pixel(&cub->mmap, i, HMAP - 1, CWHITE);
		}
		for (int i = 0; i < HMAP; i++) {
			draw_pixel(&cub->mmap, 0, i, CWHITE);
			draw_pixel(&cub->mmap, WMAP - 1, i, CWHITE);
		}
}

void draw_player(t_cub *cub)
{
    t_coord center = {WMAP / 2.0 - TSIZE / 2.0, HMAP / 2.0 - TSIZE / 2.0};
    t_coord plane_start = {center.x - cub->plane.x * 10, center.y - cub->plane.y * 10};
    t_coord plane_end = {center.x + cub->plane.x * 10, center.y + cub->plane.y * 10};
    t_coord dir_start = {center.x + cub->dir.x * 10, center.y + cub->dir.y * 10};

    draw_line(&cub->mmap, plane_start, plane_end, CBLACK);
    draw_line(&cub->mmap, dir_start, center, CRED);
}

// =========================== Raycasting ===========================

void init_ray(t_cub *cub, t_ray *ray, double camera_offset) {
    // Ray direction
    ray->rayDir.x = cub->dir.x + camera_offset * cub->plane.x;
    ray->rayDir.y = cub->dir.y + camera_offset * cub->plane.y;

    // Map position
    ray->map.x = (int)cub->player.x;
    ray->map.y = (int)cub->player.y;

    // Distance to next x or y side
    ray->d.x = (ray->rayDir.x == 0) ? INFINITY : fabs(1.0 / ray->rayDir.x);
    ray->d.y = (ray->rayDir.y == 0) ? INFINITY : fabs(1.0 / ray->rayDir.y);

    // Steps and initial sideDist
    ray->step.x = (ray->rayDir.x < 0) ? -1 : 1;
    ray->sideDist.x = (ray->rayDir.x < 0)
        ? (cub->player.x - ray->map.x) * ray->d.x
        : (ray->map.x + 1 - cub->player.x) * ray->d.x;

    ray->step.y = (ray->rayDir.y < 0) ? -1 : 1;
    ray->sideDist.y = (ray->rayDir.y < 0)
        ? (cub->player.y - ray->map.y) * ray->d.y
        : (ray->map.y + 1 - cub->player.y) * ray->d.y;
}

int get_wall_color(t_ray *ray) {
    switch (ray->side_hit) {
        case 'N': return CGREEN;
        case 'S': return CCYAN;
        case 'E': return CORANGE;
        case 'W': return CRED;
        default: return 0;
    }
}


static void calc_column_bounds(t_ray *ray) {
    ray->line_height = (int)(HWIN / ray->perpWallDist);
    ray->draw_start = fmax(0, -ray->line_height / 2 + HWIN / 2);
    ray->draw_end = fmin(HWIN - 1, ray->line_height / 2 + HWIN / 2);
}

void draw_column(t_ray *ray, t_cub *cub) {
    ray->color = get_wall_color(ray);
    calc_column_bounds(ray);
    for (int y = 0; y < HWIN; y++) {
        int color = (y >= ray->draw_start && y <= ray->draw_end) ? ray->color : CBLACK;
        draw_pixel(&cub->gameImg, ray->draw_x, y, color);
    }
}

/**
 * @brief Si le rayon touche un mur vertical, c'est soit à l'Est soit à l'Ouest
 * Si le rayon touche un mur horizontal, c'est soit au Nord soit au Sud
 */
void	assign_side_hit(t_ray *ray, bool vertical)
{
	if (vertical)
	{
		if (ray->step.x > 0)
			ray->side_hit = 'E';
		else
			ray->side_hit = 'W';
	}
	else
	{
		if (ray->step.y > 0)
			ray->side_hit = 'N';
		else
			ray->side_hit = 'S';
	}
}

/**
 * @brief Algorithme DDA (Digital Differential Analysis)
 * pour calculer la distance PERPWALLDIST entre le joueur et le mur
 */
void	dda(t_ray *ray, char **map)
{
	bool vertical;

	while (map[ray->map.y][ray->map.x] != '1') {
			vertical = ray->sideDist.x < ray->sideDist.y;
			if (vertical) {
					ray->sideDist.x += ray->d.x;
					ray->map.x += ray->step.x;
			} else {
					ray->sideDist.y += ray->d.y;
					ray->map.y += ray->step.y;
			}
			assign_side_hit(ray, vertical);
	}
	if (ray->side_hit == 'N' || ray->side_hit == 'S')
		ray->perpWallDist = ray->sideDist.y - ray->d.y;
	else
		ray->perpWallDist = ray->sideDist.x - ray->d.x;
}

void draw_ray_minimap(t_ray *ray, t_cub *cub, int x){
	t_coord	dist;
	if (x % 70 != 0)
		return ;

	dist.x = CENTER_MMAP_X + ray->rayDir.x * ray->perpWallDist * TSIZE;
	dist.y = CENTER_MMAP_Y + ray->rayDir.y * ray->perpWallDist * TSIZE;
	draw_line(&cub->mmap, (t_coord){CENTER_MMAP_X, CENTER_MMAP_Y}, (t_coord){dist.x, dist.y}, CORANGE);
}

/**
 * @brief calculer pour chaque pixel de la fenetre
 * les coordonne de rayDirX et rayDirY, puis le trace sur la fenetre.
 *
 * @param cub
 */
void raycasting(t_cub *cub) {
    t_ray ray;
    for (int x = 0; x < WWIN; x++) {
        double camera_offset = 2.0 * x / (double)WWIN - 1.0;
        ray.draw_x = x;
        init_ray(cub, &ray, camera_offset);
        dda(&ray, cub->map);
        draw_ray_minimap(&ray, cub, x);
        draw_column(&ray, cub);
    }
}

void	render(t_cub *cub)
{
	draw_minimap(cub, (t_coord){cub->player.x * TSIZE, cub->player.y * TSIZE});
	draw_player(cub);
	raycasting(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->gameImg.img, 0, 0);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->mmap.img, 40, 40);
}

int	main(void)
{
	t_cub	cub;

	init_cub(&cub);
	init_start_angle(&cub);
	event(&cub);
	print_log_debug(&cub);
	render(&cub);
	mlx_loop(cub.mlx);
	return (0);
}
