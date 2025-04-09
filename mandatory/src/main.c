/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:51:16 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 15:14:45 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/so_long.h"

int main(int argc, char *argv[])
{
	t_map_error status;
	t_game *game;

	status = validate_argument(argc, argv, &game);
	if (status != MAP_SUCCESS)
		error_exit(status);

}
