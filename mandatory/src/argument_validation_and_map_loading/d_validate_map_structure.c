/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_validate_map_struct.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:10:30 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 15:46:06 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"


t_map_error check_minimum_size(const t_map *map)
{

}

t_map_error check_if_rectangular(const t_map *map)
{

}

t_map_error check_character_validity(const t_map *map)
{

}

t_map_error validate_map_struct(t_map *map)
{	
	t_map_error status;

	if (map == NULL || map->grid == NULL)
		return (ERR_MALLOC);
	status = check_minimum_size(map);
	if (status != MAP_SUCCESS)
		return (status);
	status = check_if_rectangular(map);
	if (status != MAP_SUCCESS)	
		return (status);
	status = check_character_validity(map);
	if (status != MAP_SUCCESS)
		return (status);
	return (MAP_SUCCESS);
}
