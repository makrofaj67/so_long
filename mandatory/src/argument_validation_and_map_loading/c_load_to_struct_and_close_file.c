/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_load_to_struct_and_close_file.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:32:48 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 16:54:49 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void free_grid_rows(char **grid, int allocated_rows)
{
    while (allocated_rows > 0)
    {
        allocated_rows--;
        free(grid[allocated_rows]);
    }
    free(grid);
}

static t_map_error read_lines_to_list(int fd, t_list **lines_list, int *height)
{
    char    *line;
    t_list  *new_node;
    int     gnl_ret;

    *lines_list = NULL;
    *height = 0;
    while (1) // Loop until EOF or error
    {
        gnl_ret = get_next_line(fd, &line); // Adapt based on your GNL prototype
        if (gnl_ret == 0) // EOF
            break;
        if (gnl_ret < 0) // GNL Error
        {
            ft_lstclear(lines_list, free); // Free list created so far
            return (ERR_FILE_READ);
        }

        // If line ends with newline, potentially remove it (depends on GNL impl. and map format)
        // size_t len = ft_strlen(line);
        // if (len > 0 && line[len - 1] == '\n')
        //     line[len - 1] = '\0';

        new_node = ft_lstnew(line); // GNL allocates line, lstnew takes ownership
        if (!new_node)
        {
            free(line); // Free the line GNL returned
            ft_lstclear(lines_list, free);
            return (ERR_MALLOC);
        }
        ft_lstadd_back(lines_list, new_node);
        (*height)++;
    }
    // Ensure we didn't hit an error right at the end
    if (gnl_ret < 0)
    {
       ft_lstclear(lines_list, free);
       return (ERR_FILE_READ);
    }
    return (MAP_SUCCESS);
}

// Helper function to allocate grid and copy lines from list
static t_map_error allocate_and_fill_grid(t_list *lines_list, t_game *game)
{
    int     i;
    t_list *current;

    game->map.height = ft_lstsize(lines_list); // Get height from list size
    if (game->map.height == 0 || !lines_list) // Should have been caught by EMPTY check, but be safe
        return (ERR_MAP_EMPTY); // Or perhaps ERR_FILE_READ if unexpected

    // Assuming GNL didn't strip newline, adjust strlen if needed
    game->map.width = ft_strlen((char *)lines_list->content);
    if (game->map.width == 0) // Empty lines not allowed
         return (ERR_MAP_RECT); // Or a specific error

    // Allocate grid (array of char pointers) + 1 for NULL terminator
    game->map.grid = (char **)malloc(sizeof(char *) * (game->map.height + 1));
    if (!game->map.grid)
        return (ERR_MALLOC);

    i = 0;
    current = lines_list;
    while (current)
    {
        // Check for rectangular shape (adjust strlen if newline was stripped)
        if (ft_strlen((char *)current->content) != (size_t)game->map.width)
        {
            free_grid_rows(game->map.grid, i); // Free partially allocated rows
            return (ERR_MAP_RECT);
        }
        // Duplicate the line content into the grid
        game->map.grid[i] = ft_strdup((char *)current->content);
        if (!game->map.grid[i])
        {
            free_grid_rows(game->map.grid, i); // Free partially allocated rows
            return (ERR_MALLOC);
        }
        current = current->next;
        i++;
    }
    game->map.grid[i] = NULL; // Null-terminate the grid
    return (MAP_SUCCESS);
}

/*
 * load_to_struct_and_close_file: Reads map from fd, populates game->map, closes fd.
 * Return: MAP_SUCCESS or appropriate t_map_error code.
 */
t_map_error load_to_struct_and_close_file(int fd, t_game *game)
{
    t_list      *lines_list;
    t_map_error status;
    int         height_read; // To store height from reading phase

    // 1. Read all lines into a temporary linked list
    status = read_lines_to_list(fd, &lines_list, &height_read);
    if (status != MAP_SUCCESS)
    {
        close(fd); // Close fd on read error
        return (status);
    }

    // If read was successful but list is empty (e.g., only whitespace lines?)
    if (!lines_list)
    {
       close(fd);
       return (ERR_MAP_EMPTY); // Treat as empty map
    }

    // 2. Allocate the grid and copy lines from list
    status = allocate_and_fill_grid(lines_list, game);

    // 3. Clean up the temporary linked list (regardless of allocation success/failure)
    //    The strings were duplicated into the grid, so free the originals from the list.
    ft_lstclear(&lines_list, free);

    // 4. Close the file descriptor (ALWAYS close if it was opened)
    close(fd);

    // 5. Return the status from the allocation/filling step
    return (status);
}
