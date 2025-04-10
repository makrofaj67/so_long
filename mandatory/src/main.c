/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:51:16 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 10:30:57 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/so_long.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
    t_map_error status;
    t_game* game;

    game = (t_game*)malloc(sizeof(t_game));
    if (game == NULL)
        perror("he");
    // f_error_exit(ERR_MALLOC, NULL);

    status = validate_argument_and_load_map(argc, argv, game);
    if (status != MAP_SUCCESS)
        perror("he");
    // f_error_exit(status, game);

    // Oyun başlatma işlemleri burada yapılır
    // ...

    free(game); // Belleği serbest bırak
    return (0);
}
