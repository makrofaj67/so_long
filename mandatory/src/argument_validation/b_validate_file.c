/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_validate_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:33:56 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 15:06:41 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

t_map_error check_reach_and_emptiness(const char *file_name, int *fd_out)
{
    int     fd;
    char    buffer[1];
    ssize_t bytes_read;

    if (file_name == NULL)
        return (ERR_FILE_OPEN);
    fd = open(file_name, O_RDONLY);
    if (fd == -1)
        return (ERR_FILE_OPEN);
    bytes_read = read(fd, buffer, 1);
    if (bytes_read == 0)
    {
        close(fd);
        return (ERR_MAP_EMPTY);
    }
    else if (bytes_read == -1)
    {
        close(fd);
        return (ERR_FILE_READ);
    }
*fd_out = fd;
    return (MAP_SUCCESS);
}

t_map_error check_extension(const char *file_name)
{
	size_t name_len;
	int i;

	if (file_name == NULL)
		return(ERR_ARGS_EXT);
	name_len = ft_strlen(file_name);
	if (name_len < 4)
		return(ERR_ARGS_EXT);
	if (ft_strncmp(file_name + name_len - 4, ".ber", 4) == 0)
		return(MAP_SUCCESS);
	else
		return (ERR_ARGS_EXT);
}

t_map_error validate_file(const char *file_name, int *out_fd)
{
	t_map_error status;

	status = check_extension(file_name);
	if (status != MAP_SUCESS)
		return (status);
	status = check_reach_and_emptiness(file_name, out_fd);
	return (status);
}
