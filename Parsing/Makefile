NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT = ../greg_lib
LIBFT_LIB = $(LIBFT)/libft.a


SRCS = main.c tokenizer.c parsing.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT)
	rm -f $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	rm -f $(NAME)

re: fclean $(NAME)
