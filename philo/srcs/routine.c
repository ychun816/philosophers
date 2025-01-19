/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/16 20:44:43 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.
 */

/** GET TIME
struct timeval
{
  __time_t tv_sec; //SECONDS
  __suseconds_t tv_usec; //MICROseconds
}
*/
unsigned long   get_elapse_time()
{
    t_timeval   tv;

    gettimeoftheday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}


/** print_state 
 * @param long time; // Variable to store the current timestamp in milliseconds.
 * @param bool feast_is_stop; // Boolean flag to indicate if the simulation has ended.
 * @note LOCKING ORDER MATTERS!!
 * (1) FIRST lock print_mutex!!
 * -> Make sure 1 threads print 1 state at a time,
 * -> Threads won't interfere with 1 thread print state
 * (2) THEN lock/unlock feast_stop 
 * -> Safely access the shared feast_stop variable.
 * -> Lock/unlock to just get feast_stop_mutex
*/
void    print_state(t_philo *philo, t_state state)
{
    unsigned long    crnt_timestamp;
    bool    is_stop;

    pthread_mutex_lock(&philo->ph_table->print_mutex);

    pthread_mutex_lock(&philo->ph_table->stop_mutex);
    is_stop = &philo->ph_table->feast_stop;
    pthread_mutex_unlock(&philo->ph_table->stop_mutex);

    if (!is_stop || state == DIED)
    {
        crnt_timestamp = get_elapse_time() - (philo->ph_table->start_time);

        if (state == TAKING_FORK)
            printf("%ld %d has taken a fork\n", crnt_timestamp, philo->id);
        else if (state == EATING)
            printf(" is eating\n",crnt_timestamp, philo->id);
        else if (state == SLEEPING)
            printf(" is sleeping\n", crnt_timestamp, philo->id);
        else if (state == THINKING)
            printf(" is thinking\n", crnt_timestamp, philo->id);
        else if (state == DIED)
            printf(" died", crnt_timestamp, philo->id);
    }
    pthread_mutex_unlock(&philo->ph_table->print_mutex);
}


/** philos_routine 
 * (1) Make odd ID stop(usleep)/ even ID take forks first
 * (2) start infinite loop
 * @note
 * pthread_create(&thread, NULL, philo_routine, &philo);
 * -> passing (void *)
 * @note arg = (t_philo *)arg;
 * -> Each thread is passed a pointer to a t_philo structure
 * @note
 * Most common practice: Philosophers with odd IDs (1, 3, 5, ...) are usually made to sleep first using usleep(1000). 
 * @note
 * sleep & thinking : philo transitioss ->should be printed in routine -> control flow
 * @return
 * pthread_create function expects the function it calls to return a void *.
 * void * (though in many cases, threads don't return anything useful, so it’s just NULL).
*/
void    *philo_routine(void *arg)
{
    t_philo *philo;
    bool    is_stop;

    arg = (t_philo *)arg;
    if (philo->(id % 2))
        usleep(1000);
    while (1)
    {
        //check if stop
        pthread_mutex_lock(philo->ph_table->feast_stop);
        is_stop = philo->ph_table->feast_stop;
        pthread_mutex_unlock(philo->ph_table->feast_stop);
        if (is_stop) //break if the feast stop
            break ;
        take_forks(philo);
        eating(philo);
        sleeping(philo);
        //sleep & thinking : philo transitios
        print_state(philo, SLEEPING);
        print_state(philo, THINKING);

    }
    return (NULL);
}

/** take_forks 
 * - odd ID/ even ID take turns to take forks (1 philo needs 2 forks to eat)
 * - "taking" a fork (lock a mutex) = blocking until the mutex becomes available
 * pickup order:
 * odd ID : left, right
 * even ID : right, left
*/
void    take_forks(t_philo *philo)
{
    if (philo->id % 2)
    {
        pthread_mutex_lock(philo->left_fork);//left
        print_state(philo, TAKING_FORK);
        pthread_mutex_unlock(philo->right_fork);//right
        print_state(philo, TAKING_FORK);
    }
    else
    {
        pthread_mutex_unlock(philo->right_fork);//right
        print_state(philo, TAKING_FORK);
        pthread_mutex_lock(philo->left_fork);//left
        print_state(philo, TAKING_FORK);
    }
}

/** eating */
void    eating(t_philo *philo)
{

}

/** sleeping */
void    sleeping(t_philo *philo)
{

}

/** think */
void    thinking(t_philo *philo)
{

}