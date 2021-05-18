NAME		= webserv

CC			= clang++
CFLAGS		= -Werror -Wextra -Wall -std=c++98
RM			= rm -rf

DIR_SRCS	= srcs/
DIR_OBJS	= objs/
DIR_INC		= ./includes/
DIR_LIB		= ./libft/

F_SRCS		= main.cpp parsing/get_cgi_env.cpp request.cpp
SRCS		= $(addprefix $(DIR_SRCS), $(F_SRCS))

OBJS		= $(addprefix $(DIR_OBJS), $(F_SRCS:.cpp=.o))

LIBFT		= $(DIR_LIB)libft.a

HEADER		= -I $(DIR_INC)

all:		$(NAME)

$(DIR_OBJS)%.o: $(DIR_SRCS)%.cpp
			@mkdir -p `dirname $@`
			$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(NAME):	$(DIR_OBJS) $(OBJS)
			make -C $(DIR_LIB)
			cp $(LIBFT) ./$(NAME)
			${CC} $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT)

$(DIR_OBJS):
		mkdir $(DIR_OBJS)

clean:
		make clean -C libft
		$(RM) $(DIR_OBJS)

fclean:		clean
		make fclean -C libft
		$(RM) $(NAME)

re:		fclean all

.PHONY:     all re clean fclean
