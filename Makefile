NAME		= webserv

CC			= clang++
CFLAGS		= -Werror -Wextra -Wall -std=c++98
RM			= rm -rf

DIR_SRCS	= srcs/
DIR_OBJS	= objs/
DIR_INC		= ./includes/

F_SRCS		= main.cpp parsing_request.cpp classes/request.cpp debug.cpp read_communication.cpp parsing_config.cpp classes/Location.cpp classes/Server.cpp 

SRCS		= $(addprefix $(DIR_SRCS), $(F_SRCS))

OBJS		= $(addprefix $(DIR_OBJS), $(F_SRCS:.cpp=.o))

HEADER		= -I $(DIR_INC)

all:		$(NAME)

$(DIR_OBJS)%.o: $(DIR_SRCS)%.cpp
			@mkdir -p `dirname $@`
			$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(NAME):	$(DIR_OBJS) $(OBJS)
			${CC} $(CFLAGS) $(OBJS) -o $(NAME)

$(DIR_OBJS):
		mkdir $(DIR_OBJS)

clean:
		$(RM) $(DIR_OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:     all re clean fclean
