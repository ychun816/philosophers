/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/20 19:34:26 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** check if stop
 * Lock the death_mutex to safely access feast_stop
 * Get the current state of the simulation(feast)
 * @return
 * - true if the feast should stop
 * - false if it should continue
 */
bool	check_feast_stop(t_table *table)
{
	bool	is_stop;

	pthread_mutex_lock(&table->stop_mutex);
	is_stop = table->feast_stop;
	pthread_mutex_unlock(&table->stop_mutex);
	return (is_stop);
}

/** check has dead
 * This function checks if a philosopher has died.
 * The philosopher's life is determined by the time since their last meal.
 * @note (current_time - last_eat) > philo->ph_table->time_to_die
 * This checks if the philosopher has exceeded the time limit for eating,
 * @note
 *
	- Directly accessing philo->last_eat_time without a mutex in a multithreaded environment can potentially be dangerous
 * - In a multithreaded program,
	multiple threads may be trying to read from or write to shared data (in this case,
	last_eat_time) concurrently.
 * If one thread is reading last_eat_time while another thread is updating it,
	this can lead to race conditions,
 * where the value you read may not be the one that was intended to be used.
 * When try to access shared storage => must protect with mutex lock/unlock

	* should not directly access philo->last_meal_time without using a mutex if the last_meal_time is potentially being modified concurrently by other threads.
 * @return bool
 * true(1) / false(0)!!!!
 */
bool	check_has_dead(t_philo *philo)
{
	unsigned long	current;
	unsigned long	last_eat;
	bool			is_dead;

	current = get_current_time();
	// Lock the mutex protecting the philosopher's last meal time to safely access it
	pthread_mutex_lock(&philo->eating_mutex);
	last_eat = philo->last_eat_time;
	pthread_mutex_unlock(&philo->eating_mutex);
	// check dead
	pthread_mutex_lock(&philo->ph_table->stop_mutex);
	is_dead = philo->ph_table->feast_stop;
	if (!is_dead && (current - last_eat) > philo->ph_table->time_to_die)
		// no dead yet
	{
		philo->ph_table->feast_stop = true;
		pthread_mutex_unlock(&philo->ph_table->stop_mutex);
			// unlock right after!
		print_state(philo, DIED);
		return (true);
	}
	pthread_mutex_unlock(&philo->ph_table->stop_mutex); // unlock right after!
	return (false);                                     // 0
}

/** check  meals

	*  Check if ALL philosophers(t_table *table) have eaten the required number of times (if must_eat_count is set)
 *
 */
bool	check_finish_musteat(t_table *table)
{
	int	i;
	int	full_philo;

	i = -1;
	full_philo = 0;
	// First check if there's assigned must-eat (av[6])
	if (table->nb_must_eat == -1)
		return (false); // stop the function
	while (++i < table->nb_philo)
	{
		pthread_mutex_lock(&table->philos[i].eating_mutex);
		if (table->philos[i].eat_count >= table->nb_must_eat)
			full_philo++;
		pthread_mutex_unlock(&table->philos[i].eating_mutex);
	}
	if (full_philo == table->nb_philo)
		return (true);
	return (false);
}

/** supervisor (another extra thread!)
 * main monitor function that runs in a separate thread!

	* Continuously check the status of the philos by calling check_death and check_meals for each philosopher and the entire table,
	respectively.
 * @note while (1)
 * Important to have while(1) -> Check the whole program simotaneously
 * @note usleep(1000)
 * - Prevent busy-waiting.
 * - Allow other threads to execute.
 * - Reduce CPU usage, making the program more efficient.
 */
void	*supervisor(void *arg)
{
	t_table *table;
	int i;
	// bool is_the_end;

	table = (t_table *)arg;
	while (1)
	{
		i = -1;
		while (++i < table->nb_philo)
		{
			if (check_has_dead(&table->philos[i])) // philos[i] is value,
				//not pointer!
				return (NULL);
			if (check_finish_musteat(table))
			{
				pthread_mutex_lock(&table->stop_mutex);
				table->feast_stop = true;
				pthread_mutex_unlock(&table->stop_mutex);
				return (NULL);
				// if (check_feast_stop(table))
				// {
				//     is_the_end = true;
				//     return (NULL);
				// }
			}
		}
		usleep(1000); // notneccsary , but suggested!
	}
	return (NULL);
}