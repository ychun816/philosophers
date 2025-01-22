/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:54:01 by yilin             #+#    #+#             */
/*   Updated: 2025/01/22 11:22:32 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 0
# define FAILURE 1
# define STDERR 2
# define MAX_PHILOS 200
# define MSG_FORK "\033[38;5;223m%lu %d has taken a fork 🥄\033[0m\n"
# define MSG_EAT "\033[38;5;120m%lu %d is eating 🍛\033[0m\n"
# define MSG_SLEEP "\033[38;5;141m%lu %d is sleeping 💤\033[0m\n"
# define MSG_THINK "\033[38;5;117m%lu %d is thinking 🌙\033[0m\n"
# define MSG_DIED "\033[1;38;2;255;102;102m%lu %d died 💀\033[0m\n"

/***** STRUCTURES *****/
// struct timeval
// {
//   __time_t tv_sec;		/* Seconds.  */
//   __suseconds_t tv_usec;	/* Microseconds.  */
// };

// A fork is simply a mutex
// it can either be locked (in use by a philosopher) or unlocked (available).
// There's no additional data or properties we need to track about forks
// typedef struct s_fork
// {
//     pthread_mutex_t mutex;          // Mutex for the fork
// }   t_fork;

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;
typedef struct timeval	t_timeval;

// NO NEED DEFAULT(as 0) : Philo start THINKING right when their thread begin
// NO NEED FULL : Handle meal completion through counting rather than state
typedef enum e_state
{
	TAKING_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,	
}						t_state;

typedef struct s_philo
{
	int					id;
	int					eat_count;
	unsigned long		last_eat_time;
	t_table				*ph_table;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		eating_mutex;
}						t_philo;

typedef struct s_table
{
	int					nb_philo;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	unsigned long		time_to_think;
	int					nb_must_eat;
	unsigned long		start_time;
	bool				feast_stop;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;
	pthread_t			supervise;
}						t_table;

/* OG thoughts on s_forks stuct?
struct					s_forks
{
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_status_mutex;
	pthread_mutex_t		death_mutex;
};
If you use this structure,
Each philosopher would need access to the s_forks instance
to get their respective fork mutexes.
Here’s why this can be more complex:

a. Increased indirection
b. Redundant encapsulation
c. Complicates indexing
d. No clear benefit
The s_forks structure doesn’t add any functional advantage.
The same goal (managing mutexes for forks)
is already achieved with table->forks.
*/

/*********************/
/***** FUNCTIONS *****/
/*********************/

/***** MAIN *****/
void					handle_one_philo(t_philo *philo);
int						start_party(t_table *table);
int						join_all_threads(t_table *table);

/***** INIT *****/
int						check_args(int ac, char *av[]);
int						init_table(int ac, char *av[], t_table *table);
int						init_mutex(t_table *table);
int						init_philo(t_table *table);

/***** ROUTINE *****/
void					print_state(t_philo *philo, t_state state);
void					*philo_routine(void *arg);
void					take_forks(t_philo *philo);
void					eating(t_philo *philo);
void					sleeping(t_philo *philo);
void					give_me_a_break(unsigned long duration, t_table *table);

/***** SUPERVISOR *****/
bool					check_feast_stop(t_table *table);
bool					check_has_dead(t_philo *philo);
bool					check_finish_musteat(t_table *table);
void					*supervisor(void *arg);

/***** TOOLS *****/
int						ft_atoi(const char *str);
void					ft_putstr_fd(char *s, int fd);
void					*ft_memset(void *block, int value, size_t n);
int						ft_isdigit(int i);
unsigned long			get_current_time(void);

/***** CLEANUP *****/
void					cleanup_all(t_table *table);

#endif