/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:20:10 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 11:15:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


      
#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h> // For size_t, malloc, free, exit
# include <fcntl.h>  // For open
# include <unistd.h> // For read, b e write, close
# include <stdio.h>  // For perror
// Add MLX headers later
#include "../../lib/ft_printf/printf/ft_printf.h"
#include "../../lib/get_next_line/get_next_line.h"
// --- Error Codes Enum ---
// Ensure these values are distinct and suitable for array indexing if using the array method for messages.
// MAP_SUCCESS should ideally be 0.
typedef enum e_map_error
{
	MAP_SUCCESS = 0,
	ERR_ARGS_COUNT,
	ERR_ARGS_EXT,
	ERR_FILE_OPEN,
	ERR_FILE_READ,
	ERR_MAP_EMPTY,
	ERR_MAP_TOO_SMALL, // e.g., height < 3
	ERR_MAP_RECT,
	ERR_MAP_CHARS,
	ERR_MAP_WALLS,
	ERR_MAP_MULTIPLE_PLAYERS,
	ERR_MAP_NO_PLAYER,
	ERR_MAP_EXIT_COUNT, // For > 1 exit
	ERR_MAP_NO_EXIT,
	ERR_MAP_NO_COLLECT,
	ERR_MAP_PATH,
	ERR_MALLOC,
	// Add other errors as needed
	ERR_UNKNOWN
}	t_map_error;

// --- Structures ---
typedef struct s_map
{
	char		**grid;
	int			height;
	int			width; // Store width after validation
	int			player_total;
	int			exit_total;
	int			collectible_total;
	int			player_start_x; // Store player's starting position
	int			player_start_y;
	// Temporary coords used during char processing to pass implicitly
	int			current_x;
	int			current_y;
}	t_map;

typedef struct s_game
{
	t_map		map;
	void		*mlx_ptr; // Example for later
	void		*win_ptr;  // Example for later
	int			moves;     // Example for later
	// Add pointers to textures/images later
}	t_game;

// --- Function Prototypes (Public API) ---

// a_main.c (or similar name)
void		free_map_data(t_map *map);
t_map_error	validate_argument_and_load_map(int argc, char *argv[], t_game *game);
void		f_error_exit(t_map_error error, t_game *game); // Moved prototype here

// b_validate_file.c
t_map_error	validate_file_and_open(const char *file_path);

// c_load_map.c
t_map_error	load_to_matrix(const char *path, t_game *game);

// d_validate_structure.c
t_map_error	validate_map_structure(t_map *map);

// e_validate_logic.c
t_map_error	validate_map_logic(t_map *map);

// f_error_exit.c (message function might be static or internal)
char		*map_error_message(t_map_error error);


#endif

    



