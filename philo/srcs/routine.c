/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/22 18:04:36 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** print_state
 * @note LOCKING ORDER MATTERS!!
 * (1) FIRST lock print_mutex!!
 * -> Make sure 1 threads print 1 state at a time
 * (2) THEN check lock/unlock feast_stop (if no feast not stop||if someone died)
 * -> Safely access the shared feast_stop variable.
 * -> Lock/unlock to just get feast_stop_mutex
 */
void	print_state(t_philo *philo, t_state state)
{
	unsigned long	current_time;

	pthread_mutex_lock(&philo->ph_table->print_mutex);
	if (!check_feast_stop(philo->ph_table) || state == DIED)
	{
		current_time = get_current_time() - (philo->ph_table->start_time);
		if (state == TAKING_FORK)
			printf(MSG_FORK, current_time, philo->id);
		else if (state == EATING)
			printf(MSG_EAT, current_time, philo->id);
		else if (state == SLEEPING)
			printf(MSG_SLEEP, current_time, philo->id);
		else if (state == THINKING)
			printf(MSG_THINK, current_time, philo->id);
		else if (state == DIED)
			printf(MSG_DIED, current_time, philo->id);
	}
	pthread_mutex_unlock(&philo->ph_table->print_mutex);
}

/** philos_routine
 * @note if (philo->id % 2 == 0)
 * -> odd philo start first
 * @return
 * pthread_create(&thread, NULL, philo_routine, &philo);
 * Expects the function it calls to return void *,
 * though in many cases, threads don't return anything useful,
 * so it’s just NULL).
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->ph_table->nb_philo == 1)
	{
		handle_one_philo(philo);
		return (NULL);
	}
	else
	{
		while (!check_feast_stop(philo->ph_table))
		{
			take_forks(philo);
			eating(philo);
			print_state(philo, SLEEPING);
			give_me_a_break(philo->ph_table->time_to_sleep, philo->ph_table);
			print_state(philo, THINKING);
			give_me_a_break(philo->ph_table->time_to_think, philo->ph_table);
		}
		return (NULL);
	}
}

/** take_forks
 * - odd ID/ even ID take turns to take forks (1 philo needs 2 forks to eat)
 * - "taking" a fork (lock a mutex) = blocking until the mutex becomes available
 * ORDER:
 * odd ID : left, right
 * even ID : right, left
 */
void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
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
 * - Lock/unclock eating mustex -> increment eat_count 
 * - print sate(eating)
 * - relase forks (unlock fork mutexes)
 * @note ADD periodic check(give me a break) -> give thread a brief pause
 */
void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->eating_mutex);
	philo->last_eat_time = get_current_time();
	philo->eat_count++;
	print_state(philo, EATING);
	pthread_mutex_unlock(&philo->eating_mutex);
	give_me_a_break(philo->ph_table->time_to_eat, philo->ph_table);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/** give me a break (periodic check)
 * Prevent busy-waiting (Repeatedly doing same move) & Exhausting resources(CPU)
 * Allows the system to run other tasks in the meantime
 * Calculate the duration (current - start)
 * @note
 * The recommended time span for usleep: 500~1000
 * This provides a good compromise between CPU efficiency and responsiveness
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
}

/** sleeping */
// void	sleeping(t_philo *philo)
// {
// 	print_state(philo, SLEEPING);
// 	give_me_a_break(philo->ph_table->time_to_sleep, philo->ph_table);
// }

/** thinking */
// void    thinking(t_philo *philo)
// {
//     print_state(philo, THINKING);
// }