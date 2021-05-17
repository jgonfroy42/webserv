/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 10:41:31 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/08 18:07:58 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*dest;
	char	*source;

	i = 0;
	if (dst == 0 && src == 0)
		return (NULL);
	dest = (char *)dst;
	source = (char *)src;
	while (i < n)
	{
		(dest[i] = source[i]);
		i++;
	}
	return (dst);
}
