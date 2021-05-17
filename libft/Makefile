
SRCS	= ft_atoi.c ft_atoi_llong.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
		  ft_isdigit.c ft_isprint.c ft_memccpy.c ft_memchr.c ft_memcpy.c \
		  ft_memmove.c ft_memset.c ft_strlcat.c ft_strlcpy.c ft_strlen.c \
		  ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c ft_tolower.c \
		  ft_toupper.c ft_memcmp.c ft_calloc.c ft_strdup.c ft_substr.c \
		  ft_strjoin.c ft_strtrim.c ft_split.c ft_putchar_fd.c ft_putstr_fd.c \
		  ft_putendl_fd.c ft_putnbr_fd.c ft_strmapi.c ft_itoa.c \
		  ft_charset.c

OBJS	= 	${SRCS:.c=.o}

B_SRCS	=	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
			ft_lstadd_back_bonus.c ft_lstlast_bonus.c ft_lstdelone_bonus.c \
			ft_lstiter_bonus.c ft_lstclear_bonus.c ft_lstmap_bonus.c

B_OBJS	=	${B_SRCS:.c=.o}

NAME	=	libft.a

CC		=	gcc
RM		=	rm -f
LIB		=	ar -rcs

CFLAGS	=	-Wall -Wextra -Werror

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS} libft.h
			${LIB} ${NAME} ${OBJS}

all:		${NAME}

clean:	
			${RM} ${OBJS} ${B_OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

bonus:		${B_OBJS} ${OBJS} libft.h
			${LIB} ${NAME} ${B_OBJS} ${OBJS}

.PHONY:		all clean fclean re bonus
