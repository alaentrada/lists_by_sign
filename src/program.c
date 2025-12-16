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
	pthread_mutex_destroy(&(*list)->mtx_state);
	free((*list));
}

static	int	init_list(struct s_list **list, struct s_input *input)
{
	(*list) = NULL;
	(*list) = malloc(sizeof(struct s_list));
	if (!(*list))
		return (FAILURE);
	if (pthread_mutex_init(&(*list)->mtx_positives, NULL))
		return (free((*list)), FAILURE);
	if (pthread_mutex_init(&(*list)->mtx_negatives, NULL))
		return (pthread_mutex_destroy(&(*list)->mtx_positives), FAILURE);
	if (pthread_mutex_init(&(*list)->mtx_negatives, NULL))
		return (pthread_mutex_destroy(&(*list)->mtx_positives),
				pthread_mutex_destroy(&(*list)->mtx_negatives), FAILURE);
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

static	int	print_list(struct s_list *list)
{
	struct  s_node	*tmp;
	int				i;

	tmp = NULL;
	i = 1;
	if (!list->negatives && !list->positives)
		return (FAILURE);
	printf("NEGATIVOS:\n");
	tmp = list->negatives;
	while (tmp)
	{
		printf("%i: %i\n", i, tmp->num);
		i++;
		tmp = tmp->next;
	}
	i = 1;
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
	struct s_thread	*thread;
	struct s_list	*list;
	int				i;

	if (!init_list(&list, input))
		return (fprintf(stderr, "Error al inicializar la lista\n"), FAILURE);
	thread = NULL;
	thread = malloc(sizeof(struct s_thread) * (unsigned long)list->thread_num);
	if (!thread)
		return (free_list(&list), fprintf(stderr, "Error al asignar memoria a la estructura de los hilos"), FAILURE);
	i = 0;
	while (i < list->thread_num)
	{
		thread[i].list = list;
		thread[i].thread_index = i; 
		if (pthread_create(&list->threads_id[i], NULL, start_routine, &thread[i]))
			break ;
		i++;
	}
	if (i != list->thread_num)
	{
		pthread_mutex_lock(&list->mtx_state);
		list->state = FAILURE;
		pthread_mutex_unlock(&list->mtx_state);
	}
	while (--i >= 0)
		pthread_join(list->threads_id[i], NULL);
	if (thread->list->state == FAILURE)
		return (free_list(&list), free(thread), 
				fprintf(stderr, "Error en los hilos\n"), FAILURE);
	if (!print_list(thread->list))
		return (free_list(&list), free(thread), 
				fprintf(stderr, "Error al printear las listas\n"), FAILURE);
	free_list(&thread->list);
	free(thread);
	return (SUCCESS);
}