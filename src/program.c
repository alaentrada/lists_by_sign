#include "../inc/lists_by_sign.h"

static	void	free_list(struct s_list **list)
{
	struct s_node	*tmp;

	tmp = NULL;
	free((*list)->threads_id);
	while ((*list)->positives)
	{
		tmp = (*list)->positives->next;
		free((*list)->positives);
		(*list)->positives = tmp;
	}
	while ((*list)->negatives)
	{
		tmp = (*list)->negatives->next;
		free((*list)->negatives);
		(*list)->negatives = tmp;
	}
	pthread_mutex_destroy(&(*list)->mtx_positives);
	pthread_mutex_destroy(&(*list)->mtx_negatives);
	free((*list));
}

static  int init_list(struct s_list **list, struct s_input *input)
{
	(*list) = NULL;
	(*list) = malloc(sizeof(struct s_list));
	if (!(*list))
		return (FAILURE);
	if (pthread_mutex_init(&(*list)->mtx_positives, NULL))
		return (free((*list)), FAILURE);
	if (pthread_mutex_init(&(*list)->mtx_negatives, NULL))
		return (pthread_mutex_destroy(&(*list)->mtx_positives), FAILURE);
	(*list)->thread_num = input->thread_num;
	(*list)->num_per_thread= input->num_per_thread;
	(*list)->state = SUCCESS;
	(*list)->threads_id = NULL;
	(*list)->positives = NULL;
	(*list)->negatives = NULL;
	(*list)->threads_id = malloc(sizeof(pthread_t *restrict) * input->thread_num);
	if (!(*list)->threads_id)
		return (free_list(list), FAILURE);
	return (SUCCESS);
}

int	print_list(struct s_list *list)
{
	struct  s_node	*tmp;
	int				i;

	tmp = NULL;
	i = 0;
	if (!list->negatives || !list->positives)
		return (FAILURE);
	printf("NEGATIVOS:\n");
	tmp = list->negatives;
	while (tmp)
	{
		printf("%i: %i\n", i, tmp->num);
		i++;
		tmp = tmp->next;
	}
	i = 0;
	printf("POSITIVOS:\n");
	tmp = list->positives;
	while (tmp)
	{
		printf("%i: %i\n", i, tmp->num);
		i++;
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int program(struct s_input *input)
{
	if (!input)
		return (fprintf(stderr, "No hay input\n"), FAILURE);
	printf ("Threads: %i, Nums:%i\n", input->thread_num, input->num_per_thread);
	return (SUCCESS);

}