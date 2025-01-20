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


/** init table 
 * @note 
 * table->must_eat_count = -1;
 * -> "-1" means there is no "must eat" condition (unlimited eating until the simulation ends due to other factors, such as a philosopher's death).
 * table->feast_stop = false;
 * -> It acts as a flag to indicate whether the simulation has concluded. 
 * -> The value starts as false because the simulation hasn't started yet and will remain false while philosophers are eating, thinking, and living.
 * -> It signals that the simulation should stop, likely because one of the philosophers has died (or another termination condition has been met).
 * 
 * 
*/
int init_table(int ac, char *av[], t_table *table)
{
    ft_memset(&table, 0, sizeof(t_table)); // Zero-initialize the structure

    table->nb_philo = ft_atoi(av[1]);
    table->time_to_die = ft_atoi(av[2]);
    table->time_to_eat = ft_atoi(av[3]);
    table->time_to_sleep = ft_atoi(av[4]);
    if (ac == 6)
        table->nb_must_eat = ft_atoi(av[5]);
    else
        table->nb_must_eat = -1;//FAILURE-> no need to run check
    if (table->nb_philo <= 1 || table->time_to_die != 0 ||
        table->time_to_eat != 0 || table->time_to_sleep != 0)
        return (FAILURE);
    table->feast_stop = false;
    // table->monitor = NULL;//will init in start_party
    if (init_mutex(table))
        return (FAILURE);
    if (init_philo(table))
        return (FAILURE);
    return (SUCCESS);
}

/** INIT MUTEX
 * @note
 * You cannot use ft_memset (or memset) to initialize pthread_mutex_t
 * because pthread_mutex_t is not just a block of memory. 
 * It is a system-level object with an internal structure managed by the operating system or threading library. Simply zeroing out the memory (as ft_memset would do) is insufficient and could lead to undefined behavior when you try to use the mutex.
 * @note
 * Number of Forks = Number of Philosophers:
 * (1) malloc 
 * The table->forks array holds the mutexes for all the forks.
 * Since the number of forks depends on the number of philosophers (philo_count), 
 * its size can only be determined at runtime.
 * 
 * 
 * - Fork mutexes (table->forks): 
 * Each fork is represented by a mutex to ensure that only one philosopher can "pick up" a fork at a time. Without these mutexes, multiple threads could access the same resource simultaneously, leading to undefined behavior.
 * - Print mutex (table->print_status_mutex): 
 * Ensures that log messages (e.g., a philosopher eating or dying) are printed sequentially without overlapping from multiple threads.
 * - Death mutex (table->death_mutex): 
 * Protects access to shared data related to the simulation's termination conditions, like detecting a philosopher's death.
 * 
 * @note 
 * - Philosopher 1 uses:
 *   Left fork: forks[0]
 *   Right fork: forks[1]
 * - Philosopher 2 uses:
 *   Left fork: forks[1]
 *   Right fork: forks[0]
 * 
 * N Philosophers:
 * N Forks:
 * The array size equals nb_philo (e.g., for 5 philosophers, there are 5 forks).
 * The fork for each philosopher is defined using:
 * Left fork: forks[i]
 * Right fork: forks[(i + 1) % nb_philo]
 * This ensures the last philosopher's right fork wraps around to forks[0].
 * 
 */
int init_mutex(t_table *table)
{
    int i;

    i = -1;
    //pthread_mutex_t *forks;
    if ( table->nb_philo)
    table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
    if (!table->forks)
        return (FAILURE);
    //allocate forks
    while (++i < table->nb_philo)
        pthread_mutex_init(&table->forks[i], NULL);
    //init print mutex
    if (pthread_mutex_init(&table->print_mutex, NULL))
        return (FAILURE);
    if (pthread_mutex_init(&table->stop_mutex, NULL))
        return (FAILURE);
    return (SUCCESS);
}

/** INIT_PHILO
 * @note
 * ft_memset is not used here
 * cuz initialization involves field-specific logic, complex values
 * (e.g., pointers and get_time()), and mutex initialization.
 * A loop with explicit assignments is more suitable for this case.
 */
int init_philo(t_table *table)
{
    int i;

    i = -1;
    table->philos = malloc(sizeof(t_philo) * (table->nb_philo));
    if (!table->philos)
        return (FAILURE);
    //philo 1 | id: 1, fork: 1
    while (++i < table->nb_philo)
    {
        table->philos[i].id = i + 1; //1 philo, 2 philo
        table->philos[i].eat_count = 0; 
        table->philos[i].last_eat_time = get_current_time(); 
        table->philos[i].ph_table = table;
        table->philos[i].left_fork = &table->forks[i]; //1 fork frm left
        table->philos[i].right_fork = &table->forks[(i + 1) % (table->nb_philo)];
    }
    if (pthread_mutex_init(&table->philos[i].eating_mutex, NULL))
        return (FAILURE);
    return (SUCCESS);
}