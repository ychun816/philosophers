/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:54:01 by yilin             #+#    #+#             */
/*   Updated: 2025/01/16 20:32:50 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>
# include <limits.h>

# include <pthread.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/time.h>


# define SUCCESS 0
# define FAILURE 1

# define STDERR 2


/***** STRUCTURES *****/

// struct timeval
// {
//   __time_t tv_sec;		/* Seconds.  */
//   __suseconds_t tv_usec;	/* Microseconds.  */
// };

//A fork is simply a mutex
// it can either be locked (in use by a philosopher) or unlocked (available).
//There's no additional data or properties we need to track about forks
// typedef struct s_fork
// {
//     pthread_mutex_t mutex;          // Mutex for the fork
// }   t_fork;

typedef struct s_philo  t_philo;
typedef struct s_table  t_table;
typedef struct timeval  t_timeval;

//NO NEED DEFAULT(as 0) : Philosophers start in THINKING state immediately when their thread begins
//NO NEED FULL : Handle meal completion through counting rather than state:
typedef enum e_state
{
    TAKING_FORK,
    EATING,
    SLEEPING,
    THINKING,
    DIED,
}   t_state;

typedef struct s_philo 
{
    int             id;
    int             eat_count;
    unsigned long   last_eat_time;
    t_table  *ph_table;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t eating_mutex;
    //bool is_dead;
    //bool is_finished;
} t_philo;

typedef struct s_table 
{
    int nb_philo;
    unsigned long   time_to_die;
    unsigned long   time_to_eat;
    unsigned long   time_to_sleep;
    int nb_must_eat;
    bool    feast_stop; //used as flag: true-> stimulation should stop
    unsigned long   start_time;
    t_philo *philos;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
    pthread_t supervise;  // CHECK LATER: Add monitor thread here
} t_table;

/* OG thoughts on s_forks stuct? 
struct s_forks
{
    pthread_mutex_t *forks;
    pthread_mutex_t print_status_mutex;
    pthread_mutex_t death_mutex;
};
If you use this structure, 
each philosopher would need access to the s_forks instance to get their respective fork mutexes. Here’s why this can be more complex:

a. Increased indirection
You would need to pass the s_forks structure to every function or thread that requires access to the forks, print_status_mutex, or death_mutex.
This adds unnecessary complexity, as the table structure (t_table) already centralizes this information.
b. Redundant encapsulation
Mutexes for forks are naturally grouped in t_table. Adding another structure (s_forks) to encapsulate the same data results in redundant abstraction.
c. Complicates indexing
Instead of forks[i], 
you’d need to write forks->forks[i]. 
This extra layer adds verbosity and reduces clarity.
d. No clear benefit
The s_forks structure doesn’t add any functional advantage. 
The same goal (managing mutexes for forks) is already achieved with table->forks.
*/

/*********************/
/***** FUNCTIONS *****/
/*********************/

/***** MAIN *****/
int start_party(t_table *table);
int join_all_threads(t_table *table);

/***** INIT *****/
int init_table(int ac, char *av[], t_table *table);
int init_mutex(t_table *table);
int init_philo(t_table *table);

/***** ROUTINE *****/
void    print_state(t_philo *philo, t_state state);
void    *philo_routine(void *arg);
void    take_forks(t_philo *philo);
void    eating(t_philo *philo);
void    give_me_a_break(unsigned long duration, t_table *table);

/***** SUPERVISOR *****/
bool    check_feast_stop(t_table *table);
bool    check_has_dead(t_philo *philo);
bool    check_finish_musteat(t_table *table);
void    *supervisor(void *arg);

/***** MINILIBFT *****/
int ft_atoi(const char *str);
void    ft_putstr_fd(char *s, int fd);
void    *ft_memset(void *block, int value, size_t n);
unsigned long   get_current_time(void);

/***** CLEANUP *****/
void    cleanup_all(t_table *table);


#endif