/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houkaamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 11:24:56 by houkaamo          #+#    #+#             */
/*   Updated: 2025/11/26 11:00:53 by houkaamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

static int	read_file(int fd, char **read_buffer, int *bytes_read)
{
	*read_buffer = malloc(sizeof(char) * (size_t)(BUFFER_SIZE + 1));
	if (!*read_buffer)
		return (0); 
	*bytes_read = read(fd, *read_buffer, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free(*read_buffer);
		*read_buffer = NULL;
		return (0);
	}
	return (1);
}

static char	*fill_stash(int fd, char *stash)
{
	char	*read_buffer;
	char	*tmp;
	int		bytes_read;

	if (!read_file(fd, &read_buffer, &bytes_read))
		return (free(stash),NULL);
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
		free(read_buffer);
		if (!read_file(fd, &read_buffer, &bytes_read))
			return (free(stash), NULL);
	}
	free(read_buffer);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*tmp;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = fill_stash(fd, stash);
	if (!stash || stash[0] == '\0')
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = extract_line(stash);
	tmp = stash;
	stash = get_leftover(stash);
	free(tmp);
	return (line);
}
