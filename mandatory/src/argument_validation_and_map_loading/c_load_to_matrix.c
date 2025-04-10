
#include "../../inc/so_long.h"

// Calculates map height by reading lines.
// Note: GNL errors are not explicitly handled here, assuming GNL returns NULL on error/EOF.
static t_map_error	calculate_map_height(const char *path, t_map *map)
{
	char	*line;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ERR_FILE_OPEN);
	map->height = 0;
	line = get_next_line(fd);
	while (line)
	{
		map->height++;
		free(line);
		line = get_next_line(fd);
	}
	free(line); // Free the last NULL line potentially returned by GNL
	close(fd);
	if (map->height == 0) // Should have been caught by emptiness check, but safety
		return (ERR_MAP_EMPTY);
	return (MAP_SUCCESS);
}

// Allocates memory for the grid (array of strings).
static char	**allocate_map_grid(int height)
{
	char	**grid;

	// Allocate space for 'height' pointers + 1 for the NULL terminator
	grid = (char **)malloc(sizeof(char *) * (height + 1));
	if (grid == NULL)
		return (NULL);
	return (grid);
}

// Frees partially allocated grid on error during filling.
static void	free_grid_on_error(char **grid, int rows_filled)
{
	while (rows_filled-- > 0)
		free(grid[rows_filled]);
	free(grid);
}

// Reads the map file line by line into the allocated grid.
static t_map_error	fill_map_grid(const char *path, t_map *map)
{
	char	*line;
	int		fd;
	int		i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ERR_FILE_OPEN); // Should not happen if previous checks passed
	i = 0;
	line = get_next_line(fd);
	while (line != NULL && i < map->height)
	{
		map->grid[i] = line; // GNL already allocated the line
        // Optional: Trim newline if GNL includes it
        size_t len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
		i++;
		line = get_next_line(fd);
	}
	map->grid[i] = NULL; // Null-terminate the array of strings
	free(line); // Free potential last NULL line
	close(fd);
	if (i != map->height) // Check if read count matches expected height
	{
		free_grid_on_error(map->grid, i); // Free what was read
		map->grid = NULL;
		return (ERR_FILE_READ); // Or a specific map format error
	}
	return (MAP_SUCCESS);
}

// Orchestrates map loading: calculates height, allocates grid, fills grid.
t_map_error	load_to_matrix(const char *path, t_game *game)
{
	t_map_error	result;

	result = calculate_map_height(path, &game->map);
	if (result != MAP_SUCCESS)
		return (result);
	game->map.grid = allocate_map_grid(game->map.height);
	if (game->map.grid == NULL)
		return (ERR_MALLOC);
	result = fill_map_grid(path, &game->map);
	if (result != MAP_SUCCESS)
	{
		// If fill_map_grid failed, it should have cleaned itself up.
		// We just need to ensure the grid pointer is NULL.
		game->map.grid = NULL; // Grid already freed by fill_map_grid on error
		return (result);
	}
	// Store the width of the map (assuming rectangular validated later)
	if (game->map.height > 0 && game->map.grid[0] != NULL)
		game->map.width = ft_strlen(game->map.grid[0]);
	else
		game->map.width = 0; // Or handle as error?
	return (MAP_SUCCESS);
}

    
