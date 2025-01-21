/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/20 19:57:39 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** CLEANUP_ALL
 * @note
 * in philo struct:
 * - pthread_mutex_t *left_fork;
 * - pthread_mutex_t *right_fork;
 * -> left_fork & right_fork don’t allocate memory,
 * they are just references to elements in table->forks.
 * Once table->forks is freed, they become invalid -> NO NEED free()
 *
 * @note
 * in table struct: pthread_mutex_t *forks;
 * It's dynamically allocated with malloc during initialization.
 * After destroying all the mutexes it holds, MUST FREE the memory it uses
 *
 * @note
 * If table is NULL, trying to access table->philos or table->forks would result in a segmentation fault,
 * as the memory address NULL is not a valid location to store or access data.
 */
void	cleanup_all(t_table *table)
{
	int	i;

	i = -1;

	if (!table)// Optional check to avoid accessing a NULL table
		return ;
	// clean mutex in philo struct -> many threads(arrays)-> loop
	while (++i < table->nb_philo)
	{
		if (table->philos)
			pthread_mutex_destroy(&table->philos[i].eating_mutex);
		if (table->forks)
			pthread_mutex_destroy(&table->forks[i]);
	}
	free(table->forks);
	free(table->philos);
	// clean mutex in table struct
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->stop_mutex);
}