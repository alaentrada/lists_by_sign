#include "../inc/lists_by_sign.h"

int	isint(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

static	int	check_args(char **args)
{
	if (!args[1] || !args[2])
		return (FAILURE);
	else if (strlen(args[1]) > 4 || strlen(args[2]) > 5)
		return (FAILURE);
	else if (!isint(args[1]) || !isint(args[2]))
		return (FAILURE);
    else if (atoi(args[1]) > 1000 || atoi(args[2]) > 10000 ||
			atoi(args[1]) <= 0 || atoi(args[2]) <= 0)
		return (FAILURE);
	return (SUCCESS);
}

static	int	init_input(struct s_input **input, char **args)
{
	(*input) = NULL;
	(*input) = malloc(sizeof(struct s_input));
	if (!(*input))
		return (FAILURE);
	(*input)->thread_num = atoi(args[1]);
	(*input)->num_per_thread = atoi(args[2]);
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	struct s_input *input;

	if (argc != 3)
		return (fprintf(stderr, "Numero incorrecto de argumentos\n"), fprintf(stdout, USAGE), FAILURE);
	if (!check_args(argv))
		return (fprintf(stderr, "Argumentos incorrectos\n"), fprintf(stdout, USAGE), FAILURE);
	if (!init_input(&input, argv))
		return (fprintf(stderr, "Error al inicializar estructura input\n"), FAILURE);
	printf("Thread_num:%i Num_per_thread:%i", input->thread_num, input->num_per_thread);
	if (!program(input))
		return (free(input), FAILURE);
	free(input);
	return (SUCCESS);
}