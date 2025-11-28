/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houkaamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 11:24:56 by houkaamo          #+#    #+#             */
/*   Updated: 2025/11/26 17:39:03 by houkaamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
static void	split_stash(char *stash, char **line, char **leftover)
{
	size_t	i;

	if (!stash)
	{
		*line = NULL;
		*leftover = NULL;
		return ;
	}
	i = 0;
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
	free (stash);

}

static char	*read_and_join(int fd, char *stash)
{
	char	*read_buffer;
	char	*tmp;
	int		bytes_read;

	read_buffer = malloc(sizeof(char) * ((size_t)(BUFFER_SIZE + 1)));
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

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_and_join(fd, stash);
	split_stash(stash, &line, &stash);
	return (line);
}
