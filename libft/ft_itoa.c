/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 13:00:39 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/13 13:03:11 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_size(long int n)
{
	unsigned int	count;

	count = 1;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n / 10 != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void		recursion(long int n, char *s, unsigned int i)
{
	if (n < 0)
	{
		s[0] = '-';
		n = -n;
	}
	if (n < 10)
		s[i] = n + '0';
	else
	{
		recursion(n / 10, s, i - 1);
		s[i] = n % 10 + '0';
	}
}

char			*ft_itoa(int n)
{
	char		*s;
	size_t		size;
	long int	nbr;

	nbr = (long int)n;
	size = get_size(nbr);
	s = ft_calloc(size + 1, sizeof(char));
	if (!s)
		return (NULL);
	recursion(nbr, s, size - 1);
	return (s);
}
