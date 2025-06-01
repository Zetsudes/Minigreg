
# include "../include/minishell.h"

void	free_tab(char **tab)
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

void	print_lava_intro(void)
{
	printf(RED "\n");
	printf("     🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n");
	printf(ORANGE "        WELCOME TO THE DEPTHS OF MINIHELL\n");
	printf(RED "     🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n");

	printf(RED "               (  .      )\n");
	printf(ORANGE "          )           (              )\n");
	printf(YELLOW "                .  '   .   '  .  '  .\n");
	printf(RED "     (    , )       (.   )  (   ',    )\n");
	printf(ORANGE "      .' ) ( . )    ,  ( ,     )   ( .\n");
	printf(RED "    ) ( . ) ( . )  ) _ )     )  ( ) . )\n");
	printf(RED "           🔥   🔥   🔥   🔥   🔥\n");
	printf(RESET "\n");
}