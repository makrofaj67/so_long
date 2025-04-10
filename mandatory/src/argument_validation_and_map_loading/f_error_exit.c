/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_error_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:50:11 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 11:05:26 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

// Array of error messages corresponding to t_map_error enum values.
// IMPORTANT: The order/index MUST match the enum definition in so_long.h!
static const char	*g_error_messages[] = {
	[MAP_SUCCESS] = "Success", // Should not be printed as error
	[ERR_ARGS_COUNT] = "Incorrect number of arguments. Usage: ./so_long <map.ber>",
	[ERR_ARGS_EXT] = "Invalid file extension. Map file must end with '.ber'.",
	[ERR_FILE_OPEN] = "Cannot open map file (check path/permissions).",
	[ERR_FILE_READ] = "Cannot read map file.",
	[ERR_MAP_EMPTY] = "Map file is empty or contains no data.",
	[ERR_MAP_TOO_SMALL] = "Map is too small (min 3 rows/cols needed).",
	[ERR_MAP_RECT] = "Map is not rectangular (all rows must have same length).",
	[ERR_MAP_CHARS] = "Map contains invalid characters (only '01PEC' allowed).",
	[ERR_MAP_WALLS] = "Map must be completely surrounded by walls ('1').",
	[ERR_MAP_MULTIPLE_PLAYERS] = "Map contains more than one player start ('P').",
	[ERR_MAP_NO_PLAYER] = "Map must contain exactly one player start ('P').",
	[ERR_MAP_EXIT_COUNT] = "Map must contain exactly one exit ('E').", // Covered by NO_EXIT now
	[ERR_MAP_NO_EXIT] = "Map must contain exactly one exit ('E').",
	[ERR_MAP_NO_COLLECT] = "Map must contain at least one collectible ('C').",
	[ERR_MAP_PATH] = "Invalid path: Player cannot reach all collectibles or the exit.",
	[ERR_MALLOC] = "Memory allocation failed.",
	[ERR_UNKNOWN] = "An unknown error occurred."
	// Ensure this array has entries for ALL your error enums
};

// Returns the error message string for a given error code.
char	*map_error_message(t_map_error error)
{
	// Calculate the number of defined error messages
	int	num_messages;

	num_messages = sizeof(g_error_messages) / sizeof(char *);
	// Check if the error code is within the valid range of the array
	if (error >= MAP_SUCCESS && error < num_messages && g_error_messages[error])
	{
		return ((char *)g_error_messages[error]); // Cast const away
	}
	return ((char *)g_error_messages[ERR_UNKNOWN]); // Default to unknown
}

// Prints an error message, cleans up resources, and exits.
void	f_error_exit(t_map_error error, t_game *game)
{
	// Clean up map data first, if it exists
	if (game)
	{
		free_map_data(&game->map);
		// Add cleanup for MLX resources here later if they were initialized
		// if (game->win_ptr) mlx_destroy_window(game->mlx_ptr, game->win_ptr);
		// if (game->mlx_ptr) mlx_destroy_display(game->mlx_ptr); free(game->mlx_ptr);
		// Free textures...
	}
	// Note: 'game' itself is usually stack-allocated in main, so no free(game) needed
	// unless you malloc'd it.

	ft_putstr_fd("Error\n", STDERR_FILENO); // Print generic "Error" header to stderr
	ft_putstr_fd(map_error_message(error), STDERR_FILENO); // Print specific message
	ft_putstr_fd("\n", STDERR_FILENO);
	// Optionally print system error details using perror for specific errors
	if (error == ERR_FILE_OPEN || error == ERR_FILE_READ || error == ERR_MALLOC)
	{
		perror("System error details"); // Provides OS-level error info
	}
	exit(EXIT_FAILURE); // Terminate the program with a failure status
}