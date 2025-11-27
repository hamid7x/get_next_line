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

static char	*extract_line(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	return (ft_substr(s, 0, i));
}

static char	*get_leftover(char *s)
{
	int	i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] != '\n')
		return (NULL);
	return (ft_substr(s, i + 1, ft_strlen(s) - (i + 1)));
}

static char	*fill_stash(int fd, char *stash)
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

static t_fdnode	*manage_fdnode(t_fdnode **head, int fd, int mode)
{
	t_fdnode	*t;
	t_fdnode	*prev;

	t = *head;
	prev = NULL;
	while (t && t->fd != fd)
	{
		prev = t;
		t = t->next;
	}
	if (mode == 1 && t)
	{
		if (prev)
			return (prev->next = t->next, free(t->stash), free(t), NULL);
		else
			return (*head = t->next, free(t->stash), free(t), NULL);
	}
	if (!t && mode == 0)
	{
		t = malloc(sizeof(t_fdnode));
		if (!t)
			return (NULL);
		return (t->fd = fd, t->stash = NULL, t->next = *head,*head = t, t);
	}
	return (t);
}

char	*get_next_line(int fd)
{
	static t_fdnode	*head;
	t_fdnode		*tmp;
	char			*line;
	char			*old_stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = manage_fdnode(&head, fd, 0);
	if (!tmp)
		return (NULL);
	tmp -> stash = fill_stash(fd, tmp->stash);
	if (!tmp->stash || tmp->stash[0] == '\0')
		return (manage_fdnode(&head, fd, 1), NULL);
	line = extract_line(tmp->stash);
	if (!line)
		return (manage_fdnode(&head, fd, 1), NULL);
	old_stash = tmp->stash;
	tmp->stash = get_leftover(tmp->stash);
	free(old_stash);
	if (!tmp->stash)
		manage_fdnode(&tmp, fd, 1);
	return (line);
}
