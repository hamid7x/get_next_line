/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houkaamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 19:44:37 by houkaamo          #+#    #+#             */
/*   Updated: 2025/11/27 19:44:55 by houkaamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	split_stash(char *stash, char **line, char **leftover)
{
	size_t	i;

	i = 0;
	if (!stash)
	{
		*line = NULL;
		*leftover = NULL;
		return ;
	}
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
	{
		*line = ft_substr(stash, 0, i + 1);
		if (stash[i + 1] != '\0')
			*leftover = ft_substr(stash, i + 1, ft_strlen(stash) - (i + 1));
		else
			*leftover = NULL;
	}
	else
	{
		*line = ft_substr(stash, 0, i);
		*leftover = NULL;
	}
	free(stash);
}

static char	*read_and_join(int fd, char *stash)
{
	char	*read_buffer;
	char	*tmp;
	int		bytes_read;

	read_buffer = malloc(sizeof(char) * (((size_t)BUFFER_SIZE) + 1));
	if (!read_buffer)
		return (free(stash), NULL);
	bytes_read = read(fd, read_buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		read_buffer[bytes_read] = '\0';
		tmp = stash;
		stash = ft_strjoin(stash, read_buffer);
		free(tmp);
		if (!stash)
			return (free(read_buffer), NULL);
		if (ft_strchr(read_buffer, '\n'))
			break ;
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
	}
	if (bytes_read < 0)
		return (free(read_buffer), free(stash), NULL);
	free(read_buffer);
	return (stash);
}

static t_fdnode	*create_fdnode(t_fdnode **head, int fd)
{
	t_fdnode	*new;
	t_fdnode	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->fd == fd)
			return (tmp);
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_fdnode));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->stash = NULL;
	new->next = *head;
	*head = new;
	return (new);
}

static void	remove_fdnode(t_fdnode **head, int fd)
{
	t_fdnode	*tmp;
	t_fdnode	*prev;

	tmp = *head;
	prev = NULL;
	while (tmp && tmp -> fd != fd)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return ;
	if (prev)
		prev->next = tmp->next;
	else
		*head = tmp->next;
	free(tmp->stash);
	free(tmp);
}

char	*get_next_line(int fd)
{
	static t_fdnode	*head;
	t_fdnode		*tmp;
	char			*line;
	char			*old_stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = create_fdnode(&head, fd);
	if (!tmp)
		return (NULL);
	tmp -> stash = read_and_join(fd, tmp->stash);
	split_stash(tmp->stash, &line, &tmp->stash);
	if (!line)
		return (remove_fdnode(&head, fd), NULL);
	if (!tmp->stash)
		remove_fdnode(&head, fd);
	return (line);
}
