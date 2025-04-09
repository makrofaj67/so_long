/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:45:13 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 16:47:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/so_long.h"

typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;

t_list *ft_lstnew(void *content)
{
    t_list *new_node;

    new_node = (t_list *)malloc(sizeof(t_list));
    if (!new_node)
        return (NULL);
    new_node->content = content;
    new_node->next = NULL;
    return (new_node);
}

t_list *ft_lstlast(t_list *lst)
{
    if (!lst)
        return (NULL);
    while (lst->next)
        lst = lst->next;
    return (lst);
}

void ft_lstadd_back(t_list **lst, t_list *new_node)
{
    t_list *last_node;

    if (!new_node)
        return;
    if (!lst)
        return;
    if (*lst == NULL)
        *lst = new_node;
    else
    {
        last_node = ft_lstlast(*lst);
        last_node->next = new_node;
    }
}

int ft_lstsize(t_list *lst)
{
    int count = 0;

    while (lst)
    {
        lst = lst->next;
        count++;
    }
    return (count);
}

void ft_lstclear(t_list **lst, void (*del)(void *))
{
    t_list *current;
    t_list *next_node;

    if (!lst || !del)
        return;

    current = *lst;
    while (current)
    {
        next_node = current->next;
        del(current->content);
        free(current);
        current = next_node;
    }
    *lst = NULL;
}
