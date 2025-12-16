#include "../inc/lists_by_sign.h"

static	int	random_int(unsigned int *seed)
{
	int	num;
	int	sign;

	num = rand_r(seed);
	sign = rand_r(seed);
	if (sign < 1073741823)
		num = (num * -1) - 1;
	return (num);
}

static	int	check_n_num(struct s_list **list, int num)
{
	struct s_node	*tmp;

	tmp = (*list)->negatives;
	while (tmp)
	{
		if (tmp->num == num)
			return (SUCCESS);
		tmp = tmp->next;
	}
	return (FAILURE);
}

static	int	check_p_num(struct s_list **list, int num)
{
	struct s_node	*tmp;

	tmp = (*list)->positives;
	while (tmp)
	{
		if (tmp->num == num)
			return (SUCCESS);
		tmp = tmp->next;
	}
	return (FAILURE);
}

static	int	insert_node(struct s_list **list, unsigned int *seed)
{
	int				num;
	struct s_node	*prev, *current, *new_node;

	current = NULL;
	prev = NULL;
	new_node = NULL;
	num = random_int(seed);
	if (num < 0)
	{
		pthread_mutex_lock(&(*list)->mtx_negatives);
		if (check_n_num(list, num))
			return (pthread_mutex_unlock(&(*list)->mtx_negatives), insert_node(list, seed));
		current = (*list)->negatives;
		while (current && current->num < num)
		{
			prev = current;
			current = current->next;
		}
		new_node = malloc(sizeof(struct s_node));
		if (!new_node)
			return (fprintf(stderr, "Error al alojar memoria para nuevo nodo\n"), 
					pthread_mutex_unlock(&(*list)->mtx_negatives), FAILURE);
		new_node->num = num;
		if (!(*list)->negatives || !prev)
			(*list)->negatives = new_node;
		else
			prev->next = new_node;
		new_node->next = current;
		return (pthread_mutex_unlock(&(*list)->mtx_negatives), SUCCESS);
	}
	else
	{
		pthread_mutex_lock(&(*list)->mtx_positives);
		if (check_p_num(list, num))
			return (pthread_mutex_unlock(&(*list)->mtx_positives), insert_node(list, seed));
		current = (*list)->positives;
		while (current && current->num < num && current->num > 0)
		{
			prev = current;
			current = current->next;
		}
		new_node = malloc(sizeof(struct s_node));
		if (!new_node)
			return (fprintf(stderr, "Error al alojar memoria para nuevo nodo\n"), 
					pthread_mutex_unlock(&(*list)->mtx_positives), FAILURE);
		new_node->num = num;
		if (!(*list)->positives || !prev)
			(*list)->positives = new_node;
		else
			prev->next = new_node;
		new_node->next = current;
		return (pthread_mutex_unlock(&(*list)->mtx_positives), SUCCESS);
	}
}

void	*start_routine(void *arg)
{
	struct	s_thread	*thread;
	unsigned int		seed;
	int					i;

	thread = (struct s_thread*)arg;
	seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)thread->thread_index;
	i = 0;
	while (i < thread->list->num_per_thread)
	{
		if (!insert_node(&thread->list, &seed))
		{
			pthread_mutex_lock(&thread->list->mtx_state);
			thread->list->state == FAILURE;
			pthread_mutex_unlock(&thread->list->mtx_state);
			return (fprintf(stderr, "Error al insertar un nodo\n"), (void *)NULL);
		}
		pthread_mutex_lock(&thread->list->mtx_state);
		if (thread->list->state == FAILURE)
			return (pthread_mutex_unlock(&thread->list->mtx_state), (void *)NULL);
		else
			pthread_mutex_unlock(&thread->list->mtx_state);
		i++;
	}
	return (void *)NULL;
}