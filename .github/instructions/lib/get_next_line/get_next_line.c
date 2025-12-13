/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 15:30:01 by huozturk          #+#    #+#             */
/*   Updated: 2025/03/22 14:28:53 by huozturk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

static char	*ft_after_line(char *str)
{
	char	*newstr;
	int		j;
	int		i;

	j = 0;
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (free(str), NULL);
	newstr = ft_calloc(ft_strlen_get(str) - i + 1, sizeof(char));
	if (!newstr)
		return (free(str), NULL);
	i++;
	while (str[i] != '\0')
		newstr[j++] = str[i++];
	newstr[j] = '\0';
	return (free(str), newstr);
}

static char	*ft_line(char *str)
{
	char	*line;
	int		i;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\n')
		line = ft_calloc(i + 2, sizeof(char));
	else
		line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (free(str), NULL);
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*ft_read_fd(char *str, int fd)
{
	char	*buffer;
	int		byte_readed;

	byte_readed = 1;
	buffer = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	while (!ft_strchr_get(str, '\n') && byte_readed != 0)
	{
		byte_readed = read(fd, buffer, BUFFER_SIZE);
		if (byte_readed == -1)
			return (free(str), free(buffer), NULL);
		buffer[byte_readed] = '\0';
		str = ft_strjoin_get(str, buffer);
	}
	return (free(buffer), str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	str = ft_read_fd(str, fd);
	if (!str)
		return (NULL);
	line = ft_line(str);
	str = ft_after_line(str);
	return (line);
}
