
LIBFT_DIR = greg_lib
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIE

SRC = env/env_list.c \
	  env/env_utils.c \
	  env/handle_env.c \
	  exec/test.c \
	  exec/exec.c \
	  exec/path.c \
	  exec/exec_utils.c \
	  builtins/builtins.c \
	  builtins/builtins_utils.c \
	  builtins/cd.c \
	  builtins/export.c \
	  builtins/handle_builtin.c \
	  parsing/parsing_utils.c \
	  parsing/parsing.c \
	  parsing/tokenizer.c \
	  parsing/tokenizer_utils.c \
	  parsing/expand_var.c \

BONUS_SRC = 

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

NAME = minishell
BONUS_NAME = minishell_bonus

all: $(NAME)
bonus: $(BONUS_NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT) -o $(BONUS_NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -I $(LIBFT_DIR) -Iinclude -c $< -o $@

clean:
	$(MAKE) clean -C $(LIBFT_DIR)
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME) $(BONUS_OBJ)

re: fclean all

.PHONY: all clean fclean re
