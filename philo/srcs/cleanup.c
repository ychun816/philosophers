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

/** CLEANUP_ALL
 * @note
 * &table->forks[i], WITH & -> get address of forks[i]
 * table->forks: A POINTER -> an array of pthread_mutex_t, allocated dynamically.
 * table->forks[i]:A VALUE -> accesses an individual mutex in the array.
 * Since pthread_mutex_destroy requires a pointer to the mutex, 
 * -> Need to pass the address of table->forks[i] => "&table->forks[i]"
 * 
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
*/
void    cleanup_all(t_table *table)
{
    int i;

    i = -1;
    //clean mutex in philo struct -> many threads(arrays)-> loop 
    while (++i < table->nb_philo)
    {
        //mutex in philo struct
        pthread_mutex_destroy(&table->philos[i].meal_mutex);//table->forks[i], is a pthread_mutex_t, not a pointer to pthread_mutex_t -> need & dereference
        //mutex in table struct
        pthread_mutex_destroy(&table->forks[i]);// destroy the mutexes stored in table->forks 
    }
    //clean mutex in table struct
    pthread_mutex_destroy(&table->print_mutex);
    pthread_mutex_destroy(&table->death_mutex);
    free(table->forks);//malloced when init
    free(table->philos);//malloced when init
}