/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 15:14:32 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/08 16:19:52 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char	*tmp_s1;
	char	*tmp_s2;

	tmp_s1 = (char *)s1;
	tmp_s2 = (char *)s2;
	while (n--)
	{
		if ((unsigned char)*tmp_s1 != (unsigned char)*tmp_s2)
			return ((unsigned char)*tmp_s1 - (unsigned char)*tmp_s2);
		tmp_s1++;
		tmp_s2++;
	}
	return (0);
}
