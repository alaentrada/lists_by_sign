#ifndef PROGRAM_H
 #define PROGRAM_H

 struct s_node
 {
    int            num;
    struct s_node  *next;
 };

 struct s_list
 {
    int					thread_num;
    int					num_per_thread;
    int					state;
    pthread_t *restrict	threads_id;
    struct s_node		*positives;
    struct s_node       *negatives;
    pthread_mutex_t		mtx_positives;
    pthread_mutex_t		mtx_negatives;
 };

 struct s_thread
 {
    int				thread_index;
    struct s_list	*list;
 };

 int program(struct s_input *input);
#endif