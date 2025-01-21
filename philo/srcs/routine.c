/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/20 20:28:46 by yilin            ###   ########.fr       */
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

/** GET TIME*/
unsigned long   get_current_time(void)
{
	t_timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/** print_state
 * @param long time; // Variable to store the current timestamp in milliseconds.
 * @param bool feast_is_stop;
	// Boolean flag to indicate if the simulation has ended.
 * @note LOCKING ORDER MATTERS!!
 * (1) FIRST lock print_mutex!!
 * -> Make sure 1 threads print 1 state at a time,
 * -> Threads won't interfere with 1 thread print state
 * (2) THEN lock/unlock feast_stop
 * -> Safely access the shared feast_stop variable.
 * -> Lock/unlock to just get feast_stop_mutex
 */
void	print_state(t_philo *philo, t_state state)
{
	unsigned long	current_time;

	//lock print mutex first
    pthread_mutex_lock(&philo->ph_table->print_mutex);
    //if no feast not stop || if someone died
	if (!check_feast_stop(philo->ph_table) || state == DIED)
	{
		//get elapse time
        current_time = get_current_time() - (philo->ph_table->start_time);
		//print
        if (state == TAKING_FORK)
			printf("%lu %d has taken a fork\n", current_time, philo->id);
		else if (state == EATING)
			printf("%lu %d is eating\n", current_time, philo->id);
		else if (state == SLEEPING)
			printf("%lu %d is sleeping\n", current_time, philo->id);
		else if (state == THINKING)
			printf("%lu %d is thinking\n", current_time, philo->id);
		else if (state == DIED)
			printf("%lu %d died\n", current_time, philo->id);
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
 * Most common practice: Philosophers with odd IDs (1, 3, 5,
	...) are usually made to sleep first using usleep(1000).
 * @note
 * sleep & thinking : philo transitioss ->should be printed in routine
	-> control flow
 * @return
 * pthread_create function expects the function it calls to return a void *.
 * void * (though in many cases, threads don't return anything useful,
	so it’s just NULL).
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
    if (philo->id % 2 == 0)//philo->id % 2?
		usleep(1000);
    while (!check_feast_stop(philo->ph_table))
    {
        take_forks(philo);
        eating(philo);
        sleeping(philo);
        print_state(philo, THINKING);//thinking(philo);
    }
    return (NULL);
    //OG:
	// while (1)
	// {
	// 	// check if stop
	// 	if (check_feast_stop(philo->ph_table))
	// 		break ;
	// 	// pthread_mutex_lock(philo->ph_table->feast_stop);
	// 	// is_stop = philo->ph_table->feast_stop;
	// 	// pthread_mutex_unlock(philo->ph_table->feast_stop);
	// 	// if (is_stop) //break if the feast stop
	// 	//     break ;
	// 	take_forks(philo);
	// 	eating(philo);
	// 	// sleeping(philo);
	// 	// sleep & thinking : philo transitios
	// 	give_me_a_break(philo->ph_table->time_to_sleep, philo->ph_table);
	// 	print_state(philo, SLEEPING);
	// 	print_state(philo, THINKING);
	// }
	// return (NULL);
}

/** take_forks
 * - odd ID/ even ID take turns to take forks (1 philo needs 2 forks to eat)
 * - "taking" a fork (lock a mutex) = blocking until the mutex becomes available
 * pickup order:
 * odd ID : left, right
 * even ID : right, left
 */
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)//odd start first
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKING_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKING_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, TAKING_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, TAKING_FORK);
	}
}

/** eating
 * Lock eat
 * Count eats
 * Unlock eat
 * print sate(eating)
 * @note ADD usleep checks -> give thread a brief pause
 *
	-> Prevent busy-waiting (Repeatedly doing same move) & Exhausting resources(CPU)
 * -> Allows the system to run other tasks in the meantime.
 * after eating, put down forks: unlock left forks/right forks
 *
 */
void	eating(t_philo *philo)
{
	// start eating
	pthread_mutex_lock(&philo->eating_mutex);
	philo->last_eat_time = get_current_time();
	philo->eat_count++;
	print_state(philo, EATING);
	pthread_mutex_unlock(&philo->eating_mutex);

	// periodic check ->usleep
	give_me_a_break(philo->ph_table->time_to_eat, philo->ph_table);

    // release forks
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/** sleeping */
void    sleeping(t_philo *philo)
{
    print_state(philo, SLEEPING);
    give_me_a_break(philo->ph_table->time_to_sleep, philo->ph_table);
}

/** thinking */
// void    thinking(t_philo *philo)
// {
//     print_state(philo, THINKING);
// }

/** give me a break (periodic check)
 * Adds responsiveness and efficiency to delays by checking for global stop conditions during the wait.
 * Common Practice! Periodic check
 * Calculate the duration (current - start)
 * @param unsigned long start
 * @param unsigned long current
 * @note
* The recommended time span for usleep generally falls in the range of 500 to 1000
 * This provides a good compromise between CPU efficiency and responsiveness.
 */
void	give_me_a_break(unsigned long duration, t_table *table)
{
	unsigned long	start;
	unsigned long	current;

	start = get_current_time();
	while (!check_feast_stop(table))
    {
        current = get_current_time();
		if ((current - start) >= duration)
			break ;
		usleep(500);
    }
    // while (1)
	// {
	// 	if (check_feast_stop(table))
	// 		break ;
	// 	current = get_current_time();
	// 	if ((current - start) >= duration)
	// 		break ;
	// 	usleep(500);
	// }
}
