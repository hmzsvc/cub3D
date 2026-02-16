/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 11:32:52 by hasivaci          #+#    #+#             */
/*   Updated: 2024/11/12 12:27:49 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int	len;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (*s1 != '\0' && ft_strchr(set, *s1))
		s1++;
	len = ft_strlen(s1);
	while (len != 0 && ft_strchr(set, *(s1 + len - 1)))
		len--;
	return (ft_substr(s1, 0, len));
}
