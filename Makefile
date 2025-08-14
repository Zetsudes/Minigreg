
LIBFT_DIR = greg_lib
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIE

SRC = env/env_list.c \
	  env/env_utils.c \
	  env/handle_env.c \
	  exec/main.c \
	  exec/main_utils.c \
	  exec/exec.c \
	  exec/command_utils.c \
	  exec/path.c \
	  exec/redirections.c \
	  exec/exec_utils.c \
	  builtins/builtins.c \
	  builtins/builtins_utils.c \
	  builtins/cd.c \
	  builtins/cd_utils.c \
	  builtins/export.c \
	  builtins/export_utils.c \
	  builtins/handle_builtin.c \
	  parsing/parse_args.c \
	  parsing/parse_args_utils.c \
	  parsing/parse_core.c \
	  parsing/parse_error.c \
	  parsing/parse_redir_utils.c \
	  parsing/parse_redir.c \
	  parsing/parse_segment.c \
	  parsing/parse_utils.c \
	  lexer/lex_utils.c \
	  lexer/lex_builder.c \
	  lexer/lex_add.c \
	  lexer/lex_handlers1.c \
	  lexer/lex_handlers2.c \
	  lexer/lex_core.c \
	  expand_var/exp_dollar.c \
	  expand_var/exp_quotes.c \
	  expand_var/exp_utils.c \
	  expand_var/expand_var.c \
	  pipe/clean.c \
	  pipe/pipe_utils.c \
	  pipe/pipe.c \
	  signals/signals.c \

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
