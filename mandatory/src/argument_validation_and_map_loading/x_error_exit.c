/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:01:22 by rakman            #+#    #+#             */
/*   Updated: 2025/04/08 23:26:25 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

void print_error_message(t_map_error error_code)
{

}

void error_exit(t_map_error error_code)
{
    print_error_message(error_code);
    exit(EXIT_FAILURE);
}
