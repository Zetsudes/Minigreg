NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Répertoire de libft
LIBFT = ../greg_lib
LIBFT_LIB = $(LIBFT)/libft.a

# Fichiers sources
SRCS = main.c tokenizer.c parsing.c
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(NAME)

# Création de l'exécutable principal
$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) -I$(LIBFT) -o $(NAME) $(OBJS) $(LIBFT_LIB)

# Compilation de libft si nécessaire
$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT)

# Compilation des fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	$(MAKE) clean -C $(LIBFT)
	rm -f $(OBJS)

# Nettoyage complet
fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	rm -f $(NAME) test_parser

# Recompilation complète
re: fclean all

# --- Ajout pour test_parser ---

# Compilation et linkage du test_parser avec parser.c
test_parser: test_parser.c parsing.c parser.h tokenizer.c $(LIBFT_LIB)
	$(CC) $(CFLAGS) -I$(LIBFT) -o test_parser test_parser.c parsing.c tokenizer.c $(LIBFT_LIB)

# Lancer le test_parser
test: test_parser
	./test_parser

.PHONY: all clean fclean re test_parser test
