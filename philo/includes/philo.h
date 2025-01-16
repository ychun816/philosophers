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


/***** STRUCTURES *****/

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

//A fork is simply a mutex
// it can either be locked (in use by a philosopher) or unlocked (available).
//There's no additional data or properties we need to track about forks
// typedef struct s_fork
// {
//     pthread_mutex_t mutex;          // Mutex for the fork
// }   t_fork;

typedef struct s_philo 
{
    int             id;
    int             eat_count;
    unsigned long   last_meal_time;
    pthread_t       thread;
    pthread_mutex_t meal_mutex;
    struct s_table  *table;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
} t_philo;

typedef struct s_table 
{
    int philo_count;
    unsigned long   time_to_die;
    unsigned long   time_to_eat;
    unsigned long   time_to_sleep;
    int must_eat_count;
    bool    meal_end;
    unsigned long   start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_mutex_t death_mutex;
    t_philo *philo;
} t_table;


/*********************/
/***** FUNCTIONS *****/
/*********************/

/***** MAIN *****/


/***** ROUTINE *****/


/***** SUPERVISOR *****/



/***** MINILIBFT *****/
int ft_atoi(const char *str);
void    ft_putstr_fd(char *s, int fd);
void    *ft_memset(void *block, int value, size_t n);


#endif