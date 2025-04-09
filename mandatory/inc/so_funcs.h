/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_funcs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:44:24 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 16:04:06 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_FUNCS_H
#define SO_FUNCS_H
#include "so_structs.h"

t_map_error validate_argument(int argc, char *argv[]);

t_map_error validate_file(const char *file_name, int *out_fd);
t_map_error check_extension(const char *file_name);
t_map_error check_reach_and_emptiness(const char *file_name, int *fd_out)

t_map_error validate_map_struct(t_map *map);
t_map_error read_and_parse_map(t_map *map);
t_map_error check_minimum_size(const t_map *map);
t_map_error check_if_rectangular(const t_map *map);

t_map_error check_character_validity(const t_map *map);









#endif

