/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgonfroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 16:49:27 by jgonfroy          #+#    #+#             */
/*   Updated: 2019/11/19 15:47:23 by jgonfroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static int	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (ft_strlen(s) == 0)
		return (0);
	while (s[i + 1])
	{
		if (s[i] != c && s[i + 1] == c)
			count++;
		i++;
	}
	if (s[i] != c)
		count++;
	return (count);
}

static char	**cut_words(char **tab, char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	a;

	i = 0;
	a = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			tab[a] = ft_substr(s, start, i - start);
			if (!tab[a])
			{
				free_tab(&tab[a]);
				return (0);
			}
			a++;
		}
	}
	return (tab);
}

char		**ft_split(char const *s, char c)
{
	unsigned int	count;
	char			**tab;

	if (s == 0)
		return (0);
	count = count_words(s, c);
	tab = ft_calloc(count + 1, sizeof(char *));
	if (!tab)
		return (NULL);
	tab = cut_words(tab, s, c);
	return (tab);
}
