#include "../inc/lists_by_sign.h"

int	random_int(unsigned int *seed)
{
	int	num;
	int	sign;

	num = rand_r(seed);
	sign = rand_r(seed);
	if (sign < 1073741823)
		num = (num * -1) - 1;
	return (num);
}