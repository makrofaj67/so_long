/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_validate_map_structure.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:10:30 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 11:07:40 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"
static t_map_error	check_size_shape(t_map *map)
{
	int		y;
	size_t	first_row_len;
	size_t	current_row_len;

	if (map->height < 3) // Need at least 3 rows (walls + content)
		return (ERR_MAP_TOO_SMALL);
	if (map->grid[0] == NULL) // Should not happen if loaded correctly
		return (ERR_MAP_EMPTY); // Or other appropriate error
	first_row_len = ft_strlen(map->grid[0]);
	if (first_row_len < 3) // Need at least 3 cols (walls + content)
		return (ERR_MAP_TOO_SMALL);
	map->width = first_row_len; // Store the width
	y = 1;
	while (y < map->height)
	{
		current_row_len = ft_strlen(map->grid[y]);
		if (current_row_len != first_row_len)
			return (ERR_MAP_RECT);
		y++;
	}
	return (MAP_SUCCESS);
}

// Checks if the map is enclosed by walls ('1'). Assumes rectangular.
static t_map_error	check_walls(t_map *map)
{
	int	x;
	int	y;

	// Check top and bottom rows
	y = 0;
	while (y < map->width)
	{
		if (map->grid[0][y] != '1' || map->grid[map->height - 1][y] != '1')
			return (ERR_MAP_WALLS);
		y++;
	}
	// Check first and last columns (excluding corners already checked)
	x = 1;
	while (x < map->height - 1)
	{
		if (map->grid[x][0] != '1' || map->grid[x][map->width - 1] != '1')
			return (ERR_MAP_WALLS);
		x++;
	}
	return (MAP_SUCCESS);
}

// Updates counts for P, E, C and stores player start position.
static t_map_error	update_counts_and_find_p(char c, t_map *map)
{
	if (c == 'P')
	{
		map->player_total++;
		map->player_start_x = map->current_x; // Store position
		map->player_start_y = map->current_y;
		if (map->player_total > 1)
			return (ERR_MAP_MULTIPLE_PLAYERS);
	}
	else if (c == 'E')
	{
		map->exit_total++;
		// Check for > 1 exit done after loop
	}
	else if (c == 'C')
		map->collectible_total++;
	return (MAP_SUCCESS);
}

// Checks if the character is valid and calls update_counts.
static t_map_error	process_char(char c, int x, int y, t_map *map)
{
	if (!(c == '0' || c == '1' || c == 'P' || c == 'E' || c == 'C'))
		return (ERR_MAP_CHARS);
	map->current_x = x; // Pass coords implicitly for update_counts
	map->current_y = y;
	return (update_counts_and_find_p(c, map));
}

// Iterates through the map, checks character validity and counts items.
// Also checks final counts after iterating.
static t_map_error	check_content_and_counts(t_map *map)
{
	int			x;
	int			y;
	t_map_error	status;

	map->player_total = 0; // Initialize counts before iteration
	map->exit_total = 0;
	map->collectible_total = 0;
	x = 0;
	while (x < map->height)
	{
		y = 0;
		while (y < map->width)
		{
			status = process_char(map->grid[x][y], x, y, map);
			if (status != MAP_SUCCESS)
				return (status);
			y++;
		}
		x++;
	}
	// Check final counts after iterating through the whole map
	if (map->player_total != 1) // Must be exactly one player
		return (ERR_MAP_NO_PLAYER);
	if (map->exit_total != 1) // Must be exactly one exit
		return (ERR_MAP_NO_EXIT); // Changed from EXIT_COUNT which implies multiple
	if (map->collectible_total == 0) // Must have at least one collectible
		return (ERR_MAP_NO_COLLECT);
	return (MAP_SUCCESS);
}


// Validates the overall structure: size, shape, walls, content, counts.
t_map_error	validate_map_structure(t_map *map)
{
	t_map_error	status;

	if (map == NULL || map->grid == NULL)
		return (ERR_MALLOC); // Or appropriate error
	status = check_size_shape(map); // Checks rectangular, minimum size, sets map->width
	if (status != MAP_SUCCESS)
		return (status);
	status = check_walls(map); // Checks if surrounded by '1'
	if (status != MAP_SUCCESS)
		return (status);
	status = check_content_and_counts(map); // Checks '01PEC', counts, finds player pos
	if (status != MAP_SUCCESS)
		return (status);
	// Player start X/Y should now be set in map struct
	return (MAP_SUCCESS);
}