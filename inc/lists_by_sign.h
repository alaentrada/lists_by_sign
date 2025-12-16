#ifndef LISTS_BY_SIGN_H
 #define LISTS_BY_SIGN_H
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <ctype.h>
 #include <string.h>
 #include <time.h>
 #include <stdint.h>
 
 struct s_input
 {
	int	thread_num;
	int	num_per_thread;
 };

 #include "error.h"
 #include "program.h"

#endif