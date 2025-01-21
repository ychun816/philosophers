/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/21 12:58:30 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_input_valid(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || !str[i])
		return (false);//0
	while(str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);//0
		i++;
	}
	return (true);//1
}

/** parse n check args */
// void	check_args(int ac, char *av[])
int	check_args(int ac, char *av[])
{
	int	i;

	i = 1;
	// if (ac < 5 || ac > 6)
	// 	ft_putstr_fd("Error! Wrong number of arguments\n", STDERR);

	while (i < ac)
	{
		if (!is_input_valid(av[i]))
		{
			ft_putstr_fd("Error! Invalid Arguments\n", STDERR);
			// return ;
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * - Parse n check args
 * - Initialize structures (table, philo)
 * - Start simulation (create threads)
 * - Join threads
 * - Cleanup
}
 */
int	main(int ac, char *av[])
{
    t_table table;

	if (ac < 5 || ac > 6)
		return (ft_putstr_fd("Error! Wrong number of arguments\n", STDERR), FAILURE);

	//init table
	ft_memset(&table, 0, sizeof(t_table)); // has to be outof init_table func
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
	return (SUCCESS);s
}

/** start_party
 * pthread_create() -> 2 philo 2 threads, etc
 * 
 * 	// Initialize the start time of the simulation by calling get_time()
	// It will store the current time in table->start_time
 *
 */
int	start_party(t_table *table)
{
    int i;

	i = -1;
	table->start_time = get_current_time();
	// each philo has each thread -> loop to create
	while (++i < table->nb_philo)
	{
		// Create a philosopher thread
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine,
				&table->philos[i]))
			return (FAILURE);
	}
	// Create the monitor thread to monitor the overall simulation
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
