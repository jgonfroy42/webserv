/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:19:57 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/13 18:40:05 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = -1;
	j = 0;
	if (needle[0] == 0)
		return ((char *)haystack);
	while (haystack[++i])
	{
		if (haystack[i] == needle[0])
		{
			while (haystack[i + j] == needle[j])
			{
				if (needle[j + 1] == 0)
				{
					if (j + i < len)
						return ((char *)&haystack[i]);
					else
						return (NULL);
				}
				j++;
			}
		}
	}
	return (NULL);
}
