#include "libft.h"

char            *ft_strcpy(char *dest, const char *src)
{
        int i;

        i = 0;
        if (!dest || !src)
                return (NULL);
        while (src[i])
        {
                dest[i] = src[i];
                i++;
        }
        dest[i] = '\0';
        return (dest);
}