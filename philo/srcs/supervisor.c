/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/21 14:03:13 by yilin            ###   ########.fr       */
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
 * The philo's life is determined by the time since their last meal
 * - get last_eat_time->lock/unlock mutex to safely access
 * - get feast_stop
 * @note (current_time - last_eat) > philo->ph_table->time_to_die
 * This checks if the philosopher has exceeded the time limit for eating
 * @note
 * When try to access shared storage => must protect with mutex lock/unlock!
 * Should not directly access philo->last_meal_time without using a mutex,
 * if last_meal_time potentially modified concurrently by other threads.
 * @return true(1) / false(0)!!!!
 */
bool	check_has_dead(t_philo *philo)
{
	unsigned long	current;
	unsigned long	last_eat;

	current = get_current_time();
	pthread_mutex_lock(&philo->eating_mutex);
	last_eat = philo->last_eat_time;
	pthread_mutex_unlock(&philo->eating_mutex);
	pthread_mutex_lock(&philo->ph_table->stop_mutex);
	if (!philo->ph_table->feast_stop && (current
			- last_eat) > philo->ph_table->time_to_die)
	{
		philo->ph_table->feast_stop = true;
		pthread_mutex_unlock(&philo->ph_table->stop_mutex);
		print_state(philo, DIED);
		return (true);
	}
	pthread_mutex_unlock(&philo->ph_table->stop_mutex);
	return (false);
}

/** check_finish_must eat
 * Check if ALL philos finish the required meals(if must_eat_count is set)
 * First check if there's assigned must-eat (av[6])
 * Then Loop to check each philo (tablee->philos[i])
 */
bool	check_finish_musteat(t_table *table)
{
	int	i;
	int	full_philo;

	i = -1;
	full_philo = 0;
	if (table->nb_must_eat == -1)
		return (false);
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
 * main monitor function running in a separate thread!
 * Continuously/Symotaneously check philos status (finish meals||dead)
 * @note usleep(1000)
 * - Prevent busy-waiting.
 * - Allow other threads to execute.
 * - Reduce CPU usage, making the program more efficient.
 */
void	*supervisor(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (1)
	{
		i = -1;
		while (++i < table->nb_philo)
		{
			if (check_has_dead(&table->philos[i]))
				return (NULL);
		}
		if (check_finish_musteat(table))
		{
			pthread_mutex_lock(&table->stop_mutex);
			table->feast_stop = true;
			pthread_mutex_unlock(&table->stop_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
