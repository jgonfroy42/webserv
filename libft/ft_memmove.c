/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 10:12:03 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/13 16:53:02 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*dest;
	char	*source;

	dest = (char *)dst;
	source = (char *)src;
	if (dst == 0 && src == 0)
		return (NULL);
	if (dst < src)
	{
		while (len--)
			*dest++ = *source++;
	}
	else
	{
		dest = &dest[len - 1];
		source = &source[len - 1];
		while (len--)
			*dest-- = *source--;
	}
	return (dst);
}
