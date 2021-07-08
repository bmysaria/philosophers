SRC		= main.c philo.c philo_utils.c simulation.c

OBJS	= $(SRC:.c=.o)

OBJSB	= $(SRCB:.c=.o)

NAME	= philo

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror

all:		$(NAME)

$(NAME):    $(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJS) $(OBJSB)

fclean:		clean
			$(RM) $(NAME)
re:			fclean all

.PHONY:		all clean fclean re
