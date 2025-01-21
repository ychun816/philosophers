/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:25 by yilin             #+#    #+#             */
/*   Updated: 2025/01/21 14:07:42 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/** CLEANUP_ALL
 * // clean mutex in philo struct -> many threads(arrays)-> loop
 * // clean mutex in table struct
 * @note if (!table)
 * Optional check to avoid accessing a NULL table
 * @note
 * - pthread_mutex_t *left_fork;
 * - pthread_mutex_t *right_fork;
 * -> left_fork & right_fork don’t allocate memory,
 * they are just references to elements in table->forks.
 * Once table->forks is freed, they become invalid -> NO NEED free()
 */
void	cleanup_all(t_table *table)
{
	int	i;

	i = -1;
	if (!table)
		return ;
	while (++i < table->nb_philo)
	{
		if (table->philos)
			pthread_mutex_destroy(&table->philos[i].eating_mutex);
		if (table->forks)
			pthread_mutex_destroy(&table->forks[i]);
	}
	free(table->forks);
	free(table->philos);
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->stop_mutex);
}
