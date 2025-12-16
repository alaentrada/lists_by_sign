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

void	*start_routine(void *arg)
{
	struct	s_thread	*thread;
	unsigned int		seed;
	int					num;

	thread = (struct s_thread*)arg;
	seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)thread->thread_index;
	num = random_int(&seed);
	printf("Este es es el hilo %i con seed: %u y que ha generado el número:%i\n", thread->thread_index , seed, num);
	return (void *)NULL;
}