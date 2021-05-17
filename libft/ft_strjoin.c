/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/09 19:35:25 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/28 12:56:53 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;

	if (s1 == 0 || s2 == 0)
		return (0);
	dest = ft_calloc((ft_strlen(s1) + ft_strlen(s2) + 1), sizeof(char));
	if (!dest)
		return (0);
	ft_strlcpy(dest, s1, (size_t)ft_strlen(s1) + 1);
	ft_strlcat(dest, s2, (size_t)ft_strlen(s1) + ft_strlen(s2) + 1);
	return (dest);
}
