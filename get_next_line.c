/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagunduz <yagunduz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:29:38 by yagunduz          #+#    #+#             */
/*   Updated: 2025/09/18 18:52:23 by yagunduz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *buffer)
{
	char	*line;
	char	*nl_pos;
	char	*temp;
	int		i;

	if (!buffer[0])
		return (NULL);
	nl_pos = ft_strchr(buffer, '\n');
	if (nl_pos)
	{
		*nl_pos = '\0';
		line = ft_strjoin(buffer, "\n");
		temp = ft_strdup(nl_pos + 1);
		i = -1;
		while (temp[++i])
			buffer[i] = temp[i];
		buffer[i] = '\0';
		free(temp);
	}
	else
	{
		line = ft_strdup(buffer);
		buffer[0] = '\0';
	}
	return (line);
}

static char	*read_and_join(int fd, char *saved_data)
{
	char	*buf;
	char	*temp;
	int		bytes;

	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	bytes = 1;
	while (bytes != 0 && !ft_strchr(saved_data, '\n'))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buf);
			return (NULL);
		}
		buf[bytes] = '\0';
		temp = ft_strjoin(saved_data, buf);
		free(saved_data);
		saved_data = temp;
	}
	free(buf);
	return (saved_data);
}

char	*get_next_line(int fd)
{
	static char	*saved_data;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!saved_data)
	{
		saved_data = ft_strdup("");
		if (!saved_data)
			return (NULL);
	}
	saved_data = read_and_join(fd, saved_data);
	if (!saved_data)
		return (NULL);
	line = extract_line(saved_data);
	if (!line || !*line)
	{
		free(saved_data);
		saved_data = NULL;
		if (line)
			free(line);
		return (NULL);
	}
	return (line);
}
