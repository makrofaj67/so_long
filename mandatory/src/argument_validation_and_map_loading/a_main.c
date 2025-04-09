/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:56:49 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 16:54:33 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

void free_map_data(t_map *map)
{
    int i;

    // Güvenlik kontrolü: NULL map veya NULL grid ise bir şey yapma
    if (map == NULL || map->grid == NULL)
    {
        return;
    }

    i = 0;
    // Önce grid içindeki her bir satırı (char *) free et
    // map->height bilgisi doğru şekilde set edilmiş olmalı
    // VEYA grid NULL ile sonlandırılmışsa ona göre döngü kurulabilir.
    // NULL sonlandırma daha güvenli olabilir.
    while (map->grid[i] != NULL) // NULL sonlandırılmış grid varsayımı
    {
        free(map->grid[i]); // i. satırı free et
        map->grid[i] = NULL; // İyi pratik: free edilen pointer'ı NULL yap
        i++;
    }
    // Alternatif (height biliniyorsa):
    // while (i < map->height)
    // {
    //     if (map->grid[i] != NULL) // Ekstra kontrol
    //         free(map->grid[i]);
    //     map->grid[i] = NULL;
    //     i++;
    // }


    // Sonra grid'in kendisini (char ** dizisini) free et
    free(map->grid);

    // map struct'ını (artık geçersiz olan) grid pointer'ını NULL yap
    map->grid = NULL;

    // Width ve Height gibi diğer üyeleri sıfırlamak isteğe bağlıdır,
    // genellikle gerekmez çünkü map struct'ı ya kapsam dışına çıkacak
    // ya da free edilecek (eğer malloc ile ayrıldıysa).
    // map->width = 0;
    // map->height = 0;
}

t_map_error validate_loaded_map(t_map *map)
{
    t_map_error status;
    status = validate_map_structure(map);
    if (status != MAP_SUCCESS)
    {
        free_map_data(map);
        return (status);
    }
    status = validate_map_components(map);
    if (status != MAP_SUCCESS)
    {
        free_map_data(map);
        return (status);
    }
    status = validate_map_path(map);
    if (status != MAP_SUCCESS)
    {
        free_map_data(map);
        return (status);
    }
    return (MAP_SUCCESS);
}

t_map_error validate_argument_and_load_map(int argc, const char *argv[], t_game *game)
{
    t_map_error status;
    int         fd = -1;

    if (argc != 2)
        return (ERR_ARGS_COUNT);
    status = validate_file_and_open_file(argv[1], &fd);
    if (status != MAP_SUCCESS)
        return (status);
    status = load_to_struct_and_close_file(fd, game);
    if (status != MAP_SUCCESS)
        return (status);
    status = validate_loaded_map(&(game->map));
    if (status != MAP_SUCCESS)
        return (status);
    return (MAP_SUCCESS);
}




