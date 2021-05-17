/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:13:37 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/09 17:29:19 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	s_dst;
	size_t	s_src;
	size_t	i;
	size_t	j;

	s_dst = ft_strlen(dst);
	s_src = ft_strlen((char *)src);
	if (dstsize == 0)
		return (s_src);
	if (dstsize < s_dst)
		return (s_src + dstsize);
	i = s_dst;
	j = 0;
	while (src[j] && i < dstsize - 1)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = 0;
	return (s_src + s_dst);
}
