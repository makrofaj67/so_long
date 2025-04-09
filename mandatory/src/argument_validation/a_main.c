/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:56:49 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 14:56:52 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

t_map_error validate_map(int argc, const char *argv[], t_game *game)
{
	t_map_error status;
	int fd_out;

	fd_out = -1;
	if (argc != 2)
		return (ERR_ARGS_COUNT);
	
	status = validate_file(argv[1], &fd_out);
	if (status != MAP_SUCCESS)
		return (status);
	
	status = map

	return (MAP_SUCCESS);
}

















