/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:36:50 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/21 13:08:48 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	isset(char const *set, char c)
{
	while (*set != 0)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char			*ft_strtrim(char *s1, char const *set)
{
	size_t			start;
	size_t			size;
	char			*ret;

	if (s1 == 0 || set == 0)
		return (0);
	size = ft_strlen(s1);
	start = 0;
	while (isset(set, (char)s1[start]) == 1)
		start++;
	while (isset(set, (char)s1[size - 1]) == 1)
	{
		size--;
		if (size == 0)
			return (ft_strdup(""));
	}
	ret = ft_substr(s1, start, size - start);
	free(s1);
	return (ret);
}
