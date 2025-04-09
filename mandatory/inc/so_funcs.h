/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_funcs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:44:24 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 00:12:22 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_FUNCS_H
#define SO_FUNCS_H
#include "so_structs.h"

t_map_error validate_map(int argc, char *argv[]);
void error_exit(t_map_error error_code);
int check_extension(const char *file_name);
int check_file_reach(const char *file_name);
int is_file_empty(const char *file_name);
#endif

