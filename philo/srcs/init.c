/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/21 20:42:33 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** is_input_valid
 * @return false(0) / true(1)
 */
static bool	is_input_valid(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || !str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

/** check args */
int	check_args(int ac, char *av[])
{
	int	i;

	i = 1;
	if (ft_atoi(av[i]) > MAX_PHILOS)
	{
		ft_putstr_fd("Error! Exceed Max Number of Philosophers(200)\n", STDERR);
		return (FAILURE);
	}
	while (i < ac)
	{
		if (!is_input_valid(av[i]))
		{
			ft_putstr_fd("Error! Invalid Arguments\n", STDERR);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

/** init table
 * @note table->must_eat_count = -1;
 * -> "-1" means there is no "must eat" condition
 * (unlimited eating until the simulation ends cuz of other reasons like died)
 * @note table->feast_stop = false;
 * -> Acts as a flag to indicate if the simulation has concluded
 * -> The value starts as false cuz the simulation hasn't started yet, 
 * and will remain false while philo eating, thinking, living.
 * -> Signals the simulation should stop cuz one of the philo died
 */
int	init_table(int ac, char *av[], t_table *table)
{
	if (check_args(ac, av))
		return (FAILURE);
	table->nb_philo = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (table->time_to_eat > table->time_to_sleep)
		table->time_to_think = (table->time_to_eat - table->time_to_sleep) + 1;
	else
		table->time_to_think = 1;
	if (ac == 6)
		table->nb_must_eat = ft_atoi(av[5]);
	else
		table->nb_must_eat = -1;
	table->feast_stop = false;
	if (init_mutex(table))
		return (FAILURE);
	if (init_philo(table))
		return (FAILURE);
	return (SUCCESS);
}

/** INIT MUTEX
 * - allocate forks: 1. malloc memory block, 2. init mutex
 * - init print mutex
 * @note int print_ / stop_ mutexes
 * -> Ensures state messages are printed sequentially without overlapping
 * -> Protects access to shared data related to the termination conditions
 */
int	init_mutex(t_table *table)
{
	int	i;

	i = -1;
	if (table->nb_philo)
		table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return (FAILURE);
	while (++i < table->nb_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->forks[i]);
			return (free(table->forks), FAILURE);
		}
	}
	if (pthread_mutex_init(&table->print_mutex, NULL)
		|| pthread_mutex_init(&table->stop_mutex, NULL))
		return (cleanup_all(table), FAILURE);
	return (SUCCESS);
}

/** INIT_PHILO
 * ORDER: philo 1 | id: 1, fork: fork[0] (frm the left)
 * N Philosophers:
 * N Forks: The array size == nb_philo (e.g., 5 philosophers, 5 forks)
 * The fork for each philosopher is defined using:
 * Left fork: forks[i]
 * Right fork: forks[(i + 1) % nb_philo]
 * This ensures the last philosopher's right fork wraps around to forks[0]
 */
int	init_philo(t_table *table)
{
	int	i;

	i = -1;
	table->philos = malloc(sizeof(t_philo) * (table->nb_philo));
	if (!table->philos)
		return (FAILURE);
	while (++i < table->nb_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].eat_count = 0;
		table->philos[i].last_eat_time = get_current_time();
		table->philos[i].ph_table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% (table->nb_philo)];
		pthread_mutex_init(&table->philos[i].eating_mutex, NULL);
		if (pthread_mutex_init(&table->philos[i].eating_mutex, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&table->philos[i].eating_mutex);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

/*NOTES
 * 
 * - Philosopher 1 uses:
 *   Left fork: forks[0]
 *   Right fork: forks[1]
 * - Philosopher 2 uses:
 *   Left fork: forks[1]
 *   Right fork: forks[0]
 * 
*/
/* OGS
//INIT TABLE
int	init_table(int ac, char *av[], t_table *table)
{
	if (check_args(ac, av))
		return (FAILURE);
	// parse n init
	table->nb_philo = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->nb_must_eat = ft_atoi(av[5]);
	else
		table->nb_must_eat = -1;
	// double check input //CHECK LATER!! MAYBE NOT WORKING!
	// if (table->nb_philo <= 0 || table->time_to_die <= 0
	// 	|| table->time_to_eat <= 0 || table->time_to_sleep <= 0 ||
		//(ac == 6 && table->nb_must_eat <= 0))
	// 	return (FAILURE);
	table->feast_stop = false;
	if (init_mutex(table))
		return (FAILURE);
	if (init_philo(table))
		return (FAILURE);
	return (SUCCESS);
}

// INIT_PHILO
int	init_philo(t_table *table)
{
	int	i;

	i = -1;
	table->philos = malloc(sizeof(t_philo) * (table->nb_philo));
	if (!table->philos)
		return (FAILURE);
	while (++i < table->nb_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].eat_count = 0;
		table->philos[i].last_eat_time = get_current_time();
		table->philos[i].ph_table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% (table->nb_philo)];
	}
	if (pthread_mutex_init(&table->philos[i].eating_mutex, NULL))
	{
		while (--i >= 0)
			pthread_mutex_destroy(&table->philos[i].eating_mutex);
		return (FAILURE);
	}
	return (SUCCESS);
}
*/