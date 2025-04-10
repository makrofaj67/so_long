/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:06:46 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 09:56:27 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

void free_map_data(t_map *map)
{
    int i;

    if (map == NULL || map->grid == NULL)
        return;
    i = 0;
    while (map->grid[i] != NULL) 
    {
        free(map->grid[i]); 
        map->grid[i] = NULL; 
        i++;
    }
    free(map->grid);
    map->grid = NULL;

}

t_map_error validate_loaded_map(t_game *game)
{
    t_map_error status;

    status = validate_map_structure(&game->map);
    if (status != MAP_SUCCESS)
    {
        free_map_data(&game->map);
        return (status);
    }
    status = validate_map_logic(&game->map);
    if (status != MAP_SUCCESS)
    {
        free_map_data(&game->map);
        return (status);
    }
    return (MAP_SUCCESS);
}

t_map_error validate_argument_and_load_map(int argc, char *argv[], t_game *game)
{
    t_map_error status;
	
    if (argc != 2)
        return (ERR_ARGS_COUNT);
    status = validate_file_and_open(argv[1]);
    if (status != MAP_SUCCESS)
        return (status);
    status = load_to_matrix(argv[1], game);
    if (status != MAP_SUCCESS)
		return (status);
    status = validate_loaded_map(game);
    if (status != MAP_SUCCESS)
        return (status);
    return (MAP_SUCCESS);
}




