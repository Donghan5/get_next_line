/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: donghank <donghank@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 13:29:35 by donghank          #+#    #+#             */
/*   Updated: 2024/07/24 13:36:47 by donghank         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static ssize_t	read_files(int fd, char **buffer)
{
	char	*tmp;
	ssize_t	byte;
	char	*tmp_buffer;

	tmp = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 2));
	if (!tmp)
		return (-1);
	byte = read(fd, tmp, BUFFER_SIZE);
	while (byte > 0)
	{
		tmp[byte] = '\0';
		if (!buffer[fd])
			buffer[fd] = gnl_strdup(tmp);
		else
		{
			tmp_buffer = gnl_strjoin(buffer[fd], tmp);
			free(buffer[fd]);
			buffer[fd] = tmp_buffer;
		}
		if (gnl_strchr(buffer[fd], '\n') != NULL)
			break ;
		byte = read(fd, tmp, BUFFER_SIZE);
	}
	free(tmp);
	return (byte);
}

static void	get_free(int fd, char **buffer)
{
	if (buffer[fd])
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
}

static char	*get_line(int fd, char **buffer)
{
	char	*line;
	char	*line_pos;
	char	*tmp_buffer;
	size_t	len;

	line_pos = gnl_strchr(buffer[fd], '\n');
	if (line_pos)
	{
		len = line_pos - buffer[fd] + 1;
		line = gnl_substr(buffer[fd], 0, len);
		tmp_buffer = gnl_strdup(line_pos + 1);
		free(buffer[fd]);
		buffer[fd] = tmp_buffer;
		if (buffer[fd] && buffer[fd][0] == '\0')
			get_free(fd, buffer);
	}
	else
	{
		line = gnl_strdup(buffer[fd]);
		get_free(fd, buffer);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	ssize_t		byte;

	if (fd < 0 || OPEN_MAX <= fd || BUFFER_SIZE <= 0)
		return (NULL);
	byte = read_files(fd, buffer);
	if (byte == -1)
	{
		get_free(fd, buffer);
		return (NULL);
	}
	else if (byte == 0 && (!buffer[fd] || !*buffer[fd]))
	{
		get_free(fd, buffer);
		return (NULL);
	}
	return (get_line(fd, buffer));
}

/*
int	main(void)
{
	int		fd1;
	char	*line1;
	fd1 = open("ds.txt", O_RDONLY);
	line1 = get_next_line(fd1);
	printf("%s\n", line1);
	free(line1);
	close (fd1);
	return (0);
}
*/
