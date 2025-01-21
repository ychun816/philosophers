/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/21 14:05:04 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * - Check arg numbers
 * - Initialize structures (table, philo, mutex)
 * - Start simulation (create threads)
 * - Join threads (Must do after pthread_create)
 * - Cleanup
}
 */
int	main(int ac, char *av[])
{
	t_table	table;

	if (ac < 5 || ac > 6)
		return (ft_putstr_fd("Error! Wrong number of arguments\n", STDERR),
			FAILURE);
	ft_memset(&table, 0, sizeof(t_table));
	if (init_table(ac, av, &table))
		return (ft_putstr_fd("Initialization Failed\n", STDERR), FAILURE);
	if (start_party(&table))
	{
		ft_putstr_fd("Similation Failed! Can't Eat :(\n", STDERR);
		cleanup_all(&table);
		return (FAILURE);
	}
	if (join_all_threads(&table))
	{
		ft_putstr_fd("Thread Joining Failed\n", STDERR);
		cleanup_all(&table);
		return (FAILURE);
	}
	cleanup_all(&table);
	return (SUCCESS);
}

/** start_party
 * - Initialize the start time of the simulation to store start_time
 * - Each philo has each thread -> loop to create
 * - Create the monitor thread to monitor the overall simulation
 */
int	start_party(t_table *table)
{
	int	i;

	i = -1;
	table->start_time = get_current_time();
	while (++i < table->nb_philo)
	{
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine,
				&table->philos[i]))
			return (FAILURE);
	}
	if (pthread_create(&table->supervise, NULL, supervisor, table))
		return (FAILURE);
	return (SUCCESS);
}

/** join_all_threads */
int	join_all_threads(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_philo)
	{
		if (pthread_join(table->philos[i].thread, NULL))
			return (FAILURE);
	}
	if (pthread_join(table->supervise, NULL))
		return (FAILURE);
	return (SUCCESS);
}
