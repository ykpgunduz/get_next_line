/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagunduz <yagunduz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:29:38 by yagunduz          #+#    #+#             */
/*   Updated: 2025/09/24 13:20:58 by yagunduz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_store(int fd, char *buffer)
{
	char	*temp_buffer;
	int		bytes_read;

	temp_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!temp_buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		bytes_read = read(fd, temp_buffer, BUFFER_SIZE);
		if (bytes_read == 0)
			break ;
		if (bytes_read == -1)
			return (free(temp_buffer), free(buffer), NULL);
		temp_buffer[bytes_read] = '\0';
		buffer = ft_strjoin(buffer, temp_buffer);
	}
	free(temp_buffer);
	return (buffer);
}

static char	*extract_line(char *buffer)
{
	char	*line;
	int		index;

	if (!buffer || !*buffer)
		return (NULL);
	index = 0;
	while (buffer[index] && buffer[index] != '\n')
		index++;
	line = ft_calloc(index + 2, sizeof(char));
	index = -1;
	while (buffer[++index] && buffer[index] != '\n')
		line[index] = buffer[index];
	if (buffer[index] && buffer[index] == '\n')
		line[index++] = '\n';
	return (line);
}

static char	*update_buffer(char *buffer)
{
	char	*remaining;
	char	*old_buffer;
	int		index;

	old_buffer = buffer;
	while (*buffer && *buffer != '\n')
		buffer++;
	if (!*buffer || !*(buffer + 1))
		return (free(old_buffer), NULL);
	remaining = ft_calloc(ft_strlen(buffer++) + 1, sizeof(char));
	index = 0;
	while (*buffer)
		remaining[index++] = *buffer++;
	free(old_buffer);
	return (remaining);
}

char	*get_next_line(int fd)
{
	static char	*stored_buffer;
	char		*current_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stored_buffer)
		stored_buffer = ft_calloc(1, sizeof(char));
	stored_buffer = read_and_store(fd, stored_buffer);
	if (!stored_buffer)
		return (NULL);
	current_line = extract_line(stored_buffer);
	stored_buffer = update_buffer(stored_buffer);
	return (current_line);
}
