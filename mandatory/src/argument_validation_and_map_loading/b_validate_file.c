/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_validate_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:33:56 by rakman            #+#    #+#             */
/*   Updated: 2025/04/10 11:09:06 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"
static t_map_error	check_reach_and_emptiness(const char *file_name)
{
	int		fd;
	char	buffer[1];
	ssize_t	bytes_read;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (ERR_FILE_OPEN);
	bytes_read = read(fd, buffer, 1);
	close(fd); // Close the file descriptor immediately after use
	if (bytes_read == 0)
		return (ERR_MAP_EMPTY);
	if (bytes_read == -1)
		return (ERR_FILE_READ); // Error during read
	return (MAP_SUCCESS);
}

// Checks if the file extension is ".ber".
static t_map_error	check_extension(const char *file_name)
{
	size_t	name_len;
	size_t	ext_len;
	char	*extension;

	extension = ".ber";
	ext_len = ft_strlen(extension);
	if (file_name == NULL) // Should not happen if argc check passed, but good practice
		return (ERR_ARGS_EXT);
	name_len = ft_strlen(file_name);
	if (name_len <= ext_len) // Must be longer than ".ber"
		return (ERR_ARGS_EXT);
	// Compare the end of the file_name with ".ber"
	if (ft_strncmp(file_name + name_len - ext_len, extension, ext_len) == 0)
		return (MAP_SUCCESS);
	else
		return (ERR_ARGS_EXT);
}

// Validates file extension and checks if it's readable and not empty.
t_map_error	validate_file_and_open(const char *file_path)
{
	t_map_error	status;

	status = check_extension(file_path);
	if (status != MAP_SUCCESS)
		return (status);
	status = check_reach_and_emptiness(file_path); // Checks read permission & empty
	return (status);
}

